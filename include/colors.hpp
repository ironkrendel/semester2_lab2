#pragma once

#include <string>

constexpr std::string_view ANSI_COLOR_RED = "\x1b[31m";
constexpr std::string_view ANSI_COLOR_GREEN = "\x1b[32m";
constexpr std::string_view ANSI_COLOR_YELLOW = "\x1b[33m";
constexpr std::string_view ANSI_COLOR_BLUE = "\x1b[34m";
constexpr std::string_view ANSI_COLOR_MAGENTA = "\x1b[35m";
constexpr std::string_view ANSI_COLOR_CYAN = "\x1b[36m";
constexpr std::string_view ANSI_COLOR_BRIGHT_RED = "\x1b[91m";
constexpr std::string_view ANSI_COLOR_BRIGHT_GREEN = "\x1b[92m";
constexpr std::string_view ANSI_COLOR_BRIGHT_YELLOW = "\x1b[93m";
constexpr std::string_view ANSI_COLOR_BRIGHT_BLUE = "\x1b[94m";
constexpr std::string_view ANSI_COLOR_BRIGHT_MAGENTA = "\x1b[95m";
constexpr std::string_view ANSI_COLOR_BRIGHT_CYAN = "\x1b[96m";
constexpr std::string_view ANSI_COLOR_RESET = "\x1b[0m";