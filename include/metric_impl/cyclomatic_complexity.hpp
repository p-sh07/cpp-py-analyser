#pragma once
#include <unistd.h>

#include <algorithm>
#include <array>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <ranges>
#include <sstream>
#include <string>
#include <variant>
#include <vector>

#include "metric.hpp"

namespace analyser::metric::metric_impl {
using std::literals::operator""sv;
namespace vws = std::views;
namespace rgs = std::ranges;

struct CyclomaticComplexityMetric: IMetric {
protected:
    MetricResult::ValueType CalculateImpl(const function::Function& f) const override;
    std::string Name() const override;

private:
    static constexpr std::array<std::string_view, 12> cycle_tags_ = {
        "if_statement"sv, "else_clause"sv, "elif_clause"sv, "while_statement"sv, "for_statement"sv, "try_statement"sv,
        "except_clause"sv, "finally_clause"sv, "match_statement"sv, "case_pattern"sv, "assert_statement"sv, "conditional_expression"sv
    };

    static bool MatchCycleTag(std::string_view line_with_identifier);
};

} // namespace analyser::metric::metric_impl
