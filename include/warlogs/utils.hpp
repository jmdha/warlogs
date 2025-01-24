#pragma once

#include <string_view>
#include <vector>

std::vector<std::string_view> Split(const std::string_view, const char delim = ',');
