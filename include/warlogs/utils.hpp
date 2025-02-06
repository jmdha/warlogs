#pragma once

#include <optional>
#include <string_view>
#include <vector>
#include <chrono>

namespace warlogs {
std::vector<std::string_view> Split(std::string_view, const char delim = ',') noexcept;
std::optional<std::size_t> CharIdx(std::string_view, char, unsigned int) noexcept;
std::string_view Trim(std::string_view) noexcept;
std::time_t Timestamp(std::string_view) noexcept;
} // namespace warlogs
