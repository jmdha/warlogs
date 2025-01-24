#include "warlogs/event.hpp"
#include "warlogs/utils.hpp"
#include <iostream>
#include <stdexcept>
#include <string_view>
#include <vector>

namespace Event {
// A log event starts with a date, which can be skipped by skipping 3 spaces
std::size_t FindStart(const std::string &str) {
  unsigned int spaces = 0;

  for (std::size_t i = 0; i < str.size(); i++)
    if (str[i] == ' ')
      if (++spaces > 2)
        return i;

  throw std::invalid_argument("Input does not contain three spaces");
}

Event Parse(const std::string &str) {
  const std::size_t start = FindStart(str);
  const std::vector<std::string_view> tokens = Split(&str[start], ',');
  for (const auto &tok : tokens)
    std::cout << tok << '|';
  std::cout << std::endl;
  throw std::logic_error("Hello");
}
} // namespace Event
