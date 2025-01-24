#pragma once

#include <string>
#include <variant>

namespace Event {
enum class Kind { Undefined, Version, ZoneChange, MapChange };

struct Version {
  unsigned int log;   // Which logging version
  unsigned int major; // Major of build version
  unsigned int minor; // Minor of build version
  unsigned int patch; // Patch of build version
  bool advanced;      // Is advanced logging enabled
};

struct ZoneChange {
  unsigned int instance;   // Instance ID
  unsigned int difficulty; // Difficulty ID
  std::string name;        // Zone name
};

struct MapChange {
  unsigned int id;             // Map ID
  std::string name;            // Map name
  unsigned int x0, y0, x1, y1; // Map bounds
};

using Event = std::variant<Version, ZoneChange, MapChange>;

Event Parse(const std::string &);
}; // namespace Event
