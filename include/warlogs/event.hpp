#pragma once

#include <ctime>
#include <string>
#include <variant>

namespace warlogs {
struct Undefined {
  std::string_view event;
};

// COMBAT_LOG_VERSION,X,ADVANCED_LOG_ENABLED,X,BUILD_VERSION,X.X.X,PROJECT_ID,X
struct Version {
  unsigned int log;   // Which logging version
  unsigned int major; // Major of build version
  unsigned int minor; // Minor of build version
  unsigned int patch; // Patch of build version
  bool advanced;      // Is advanced logging enabled
};

// ZONE_CHANGE, instanceID, zoneName, difficultyID
struct ZoneChange {
  unsigned int instance;   // Instance ID
  unsigned int difficulty; // Difficulty ID
  std::string_view name;   // Zone name
};

// MAP_CHANGE, uiMapID, uiMapName, x0, x1, y0, y1
struct MapChange {
  unsigned int id;             // Map ID
  std::string_view name;       // Map name
  unsigned int x0, y0, x1, y1; // Map bounds
};

// UNIT_DIED, ?, ?, ?, ?, unitID, unitName, ?, recapID, uncounsciousOnDeath
struct UnitDied {
  std::string_view id;
  std::string_view name;
};

struct PartyKill {};

struct SpellCastSucess {};

struct SpellAuraApplied {};

struct SpellSummon {};

struct SpellDamage {};

struct SpellInstakill {};

struct SpellAuraRemoved {};

struct SpellAuraRefresh {};

struct SpellPeriodicHeal {};

struct SpellEnergize {};

struct SpellPeriodicDamage {};

struct SpellHeal {};

using Event = std::variant<Undefined, Version, ZoneChange, MapChange, UnitDied, PartyKill,
                           SpellCastSucess, SpellSummon, SpellInstakill, SpellEnergize, SpellHeal,
                           SpellDamage, SpellPeriodicDamage, SpellPeriodicHeal, SpellAuraApplied,
                           SpellAuraRefresh, SpellAuraRemoved>;

std::pair<std::time_t, Event> Parse(const std::string &);

const std::string &Name(const Event &);
}; // namespace warlogs
