#include "warlogs/utils.hpp"
#include <cstring>

namespace warlogs {
std::vector<std::string_view> Split(const std::string_view str, const char delim) {
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

std::optional<std::size_t> CharIdx(const std::string_view str, char ch, unsigned int count) {
  unsigned int found = 0;

  for (std::size_t i = 0; i < str.size(); i++)
    if (str.at(i) == ch)
      if (++found > 2)
        return i + 1;

  return {};
}

std::string_view Trim(const std::string_view str) {
  return str.substr(1, str.size() - 2);
}

} // namespace warlogs
