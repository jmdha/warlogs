#include "warlogs/event.hpp"
#include "warlogs/utils.hpp"
#include <stdexcept>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace Event {
Event ParseVersion(const std::vector<std::string_view> &);
Event ParseZoneChange(const std::vector<std::string_view> &);
Event ParseMapChange(const std::vector<std::string_view> &);

const std::unordered_map<std::string_view, Kind> MAP = {
    {"COMBAT_LOG_VERSION", Kind::Version},
    {"ZONE_CHANGE", Kind::ZoneChange},
    {"MAP_CHANGE", Kind::MapChange},
};
// A log event starts with a date, which can be skipped by skipping 3 spaces
std::size_t FindStart(const std::string &str) {
  unsigned int spaces = 0;

  for (std::size_t i = 0; i < str.size(); i++)
    if (str[i] == ' ')
      if (++spaces > 2)
        return i + 1;

  throw std::invalid_argument("Input does not contain three spaces");
}

Event Parse(const std::string &str) {
  const std::size_t start = FindStart(str);
  const std::vector<std::string_view> tokens = Split(&str[start], ',');
  const std::string_view &event_name = tokens.at(0);
  const auto event_kind = MAP.find(event_name);
  if (event_kind == MAP.end())
    return Undefined{.event = event_name};
  switch (event_kind->second) {
  case Kind::Version:
    return ParseVersion(tokens);
  case Kind::ZoneChange:
    return ParseZoneChange(tokens);
  case Kind::MapChange:
    return ParseMapChange(tokens);
    break;
  default:
    break;
  }
  return Undefined{.event = "INTERNAL_ERROR"};
}

Event ParseVersion(const std::vector<std::string_view> &tokens) {
    const unsigned int log = atoi(tokens[1].data());
    const bool advanced = atoi(tokens[3].data());
    const std::vector<std::string_view> build = Split(tokens[5], '.');
    const unsigned major = atoi(build[0].data());
    const unsigned minor = atoi(build[1].data());
    const unsigned patch = atoi(build[2].data());
    return Version {
        .log = log,
        .major = major,
        .minor = minor,
        .patch = patch,
        .advanced = advanced
    };
}

Event ParseZoneChange(const std::vector<std::string_view> &tokens){}

Event ParseMapChange(const std::vector<std::string_view> &tokens){}
} // namespace Event
