#include <functional>
#include <stdexcept>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "warlogs/event.hpp"
#include "warlogs/utils.hpp"

namespace warlogs {
Event ParseVersion(const std::vector<std::string_view> &);
Event ParseZoneChange(const std::vector<std::string_view> &);
Event ParseMapChange(const std::vector<std::string_view> &);
Event ParseUnitDied(const std::vector<std::string_view> &);
Event ParsePartyKill(const std::vector<std::string_view> &);

const std::unordered_map<std::string_view,
                         std::function<Event(const std::vector<std::string_view>)>>
    MAP = {{"COMBAT_LOG_VERSION", ParseVersion},
           {"ZONE_CHANGE", ParseZoneChange},
           {"MAP_CHANGE", ParseMapChange},
           {"UNIT_DIED", ParseUnitDied},
           {"PARTY_KILL", ParsePartyKill}};

Event Parse(const std::string &str) {
  const std::optional<std::size_t> start = CharIdx(str, ' ', 3);
  if (!start.has_value())
    throw std::invalid_argument("Invalid input");
  const std::vector<std::string_view> tokens = Split(&str[start.value()], ',');
  const std::string_view &event_name = tokens.at(0);
  const auto event_kind = MAP.find(event_name);

  if (event_kind == MAP.end())
    return Undefined{.event = event_name};
  else
    return event_kind->second(tokens);
}

Event ParseVersion(const std::vector<std::string_view> &tokens) {
  const unsigned int log = atoi(tokens[1].data());
  const bool advanced = atoi(tokens[3].data());
  const std::vector<std::string_view> build = Split(tokens[5], '.');
  const unsigned major = atoi(build[0].data());
  const unsigned minor = atoi(build[1].data());
  const unsigned patch = atoi(build[2].data());
  return Version{.log = log, .major = major, .minor = minor, .patch = patch, .advanced = advanced};
}

Event ParseZoneChange(const std::vector<std::string_view> &tokens) {
  const unsigned int instance = atoi(tokens[1].data());
  const unsigned int difficulty = atoi(tokens[3].data());
  const std::string_view name = Trim(tokens[2]);
  return ZoneChange{.instance = instance, .difficulty = difficulty, .name = name};
}

Event ParseMapChange(const std::vector<std::string_view> &tokens) {
  const unsigned int id = atoi(tokens[1].data());
  const std::string_view name = Trim(tokens[2]);
  const unsigned int x0 = atoi(tokens[3].data());
  const unsigned int x1 = atoi(tokens[4].data());
  const unsigned int y0 = atoi(tokens[5].data());
  const unsigned int y1 = atoi(tokens[6].data());
  return MapChange{.id = id, .name = name, .x0 = x0, .y0 = y0, .x1 = x1, .y1 = y1};
}

Event ParseUnitDied(const std::vector<std::string_view> &tokens) {
  const std::string_view id = tokens[5];
  const std::string_view name = Trim(tokens[6]);
  return UnitDied{.id = id, .name = name};
}

Event ParsePartyKill(const std::vector<std::string_view> &tokens) { return PartyKill{}; }
} // namespace warlogs
