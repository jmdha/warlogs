#include "warlogs/utils.hpp"
#include <cstring>

std::vector<std::string_view> Split(const std::string_view str,
                                    const char delim) {
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
                                     str.size() - indexCommaToRightOfColumn -
                                         1);
  result.push_back(finalColumn);
  return result;
}
