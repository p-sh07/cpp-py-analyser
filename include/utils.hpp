#pragma once

#include <charconv>
#include <stdexcept>
#include <string>
#include <string_view>

namespace utils {
inline int ToInt(std::string_view value) {
    int result{};

    //remove leading whitespace
    while (!value.empty() && value[0] == ' ') {
        value.remove_prefix(1);
    }
    if (value.empty()) {
        throw std::runtime_error("Trying to convert empty string to int");
    }

    auto [parse_end_ptr, error_code] = std::from_chars(value.begin(), value.end(), result);

    if (error_code != std::errc{} || parse_end_ptr != value.data() + value.size()) {
        throw std::invalid_argument("Cannot convert '" + std::string(value) + "' to integral");
    }
    return result;
}

template<typename It>
int ToInt(It start, It finish) {
    int result{};

    //remove leading whitespace
    while (start != finish && *start == ' ') {
        ++start;
    }
    if (start == finish) {
        throw std::runtime_error("Trying to convert empty string to int");
    }

    auto [parse_end_ptr, error_code] = std::from_chars(start, finish, result);
    if (error_code != std::errc{} || parse_end_ptr != finish) {
        throw std::invalid_argument("Cannot convert range to integral");
    }
    return result;
}

template<typename Rg>
auto GetStringViewLines(Rg&& range, const char delim = '\n') {
    return range | std::views::split(delim) | std::views::transform([](auto&& rg) { return std::string_view{rg}; });
}
}
