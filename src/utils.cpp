#include <cstring>
#include <iostream>

#include "warlogs/utils.hpp"

namespace warlogs {
std::vector<std::string_view> Split(std::string_view str, const char delim) noexcept {
  std::vector<std::string_view> result;

  int indexCommaToLeftOfColumn = 0;
  int indexCommaToRightOfColumn = -1;

  for (int i = 0; i < static_cast<int>(str.size()); i++) {
    if (str[i] == delim) {
      indexCommaToLeftOfColumn = indexCommaToRightOfColumn;
      indexCommaToRightOfColumn = i;
      int index = indexCommaToLeftOfColumn + 1;
      int length = indexCommaToRightOfColumn - index;

      std::string_view column(str.data() + index, length);
      result.push_back(column);
    }
  }
  const std::string_view finalColumn(str.data() + indexCommaToRightOfColumn + 1,
                                     str.size() - indexCommaToRightOfColumn - 1);
  result.push_back(finalColumn);
  return result;
}

std::optional<std::size_t> CharIdx(std::string_view str, char ch, unsigned int count) noexcept {
  unsigned int found = 0;

  for (std::size_t i = 0; i < str.size(); i++)
    if (str.at(i) == ch)
      if (++found > 2)
        return i + 1;

  return {};
}

std::string_view Trim(std::string_view str) noexcept { return str.substr(1, str.size() - 2); }

std::time_t Timestamp(std::string_view sv) noexcept {
  int day, month, year, hours, mins, secs;
  sscanf(sv.data(), "%d/%d/%d %d:%d:%d", &month, &day, &year, &hours, &mins, &secs);
  std::tm tm = {
      .tm_sec = secs,
      .tm_min = mins,
      .tm_hour = hours + 1,
      .tm_mday = day,
      .tm_mon = month - 1,
      .tm_year = year - 1900,
  };
  return std::mktime(&tm);
}
} // namespace warlogs
