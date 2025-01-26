#pragma once

#include <optional>
#include <string_view>
#include <vector>

namespace warlogs {
std::vector<std::string_view> Split(const std::string_view, const char delim = ',');
std::optional<std::size_t> CharIdx(const std::string_view, char, unsigned int);
std::string_view Trim(const std::string_view);
} // namespace warlogs
