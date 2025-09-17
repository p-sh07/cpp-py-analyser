#include "metric_impl/cyclomatic_complexity.hpp"

#include <algorithm>
#include <array>
#include <ranges>
#include <string>

#include "utils.hpp"

// #include "../../../../../../../.conan2/p/b/gtestf159c44e11705/p/include/gtest/internal/gtest-param-util.h"

namespace analyser::metric::metric_impl {

//if/else, elif, while, for, try/catch/finally, match/case, assert, ternary operator.
MetricResult::ValueType CyclomaticComplexityMetric::CalculateImpl(const function::Function& f) const {
    return static_cast<MetricResult::ValueType>(
        rgs::distance(
            utils::GetStringViewLines(f.ast, '(') //split into lines by (
            | vws::filter(MatchCycleTag)
        )
    );
}

std::string CyclomaticComplexityMetric::Name() const {
    return "Cyclomatic Complexity";
}

bool CyclomaticComplexityMetric::MatchCycleTag(std::string_view line_with_identifier) {
    //discard lines that are empty or did not start with '(' before split
    if(line_with_identifier.empty() || line_with_identifier[0] == ' ') {
        return false;
    }

    return rgs::any_of(cycle_tags_, [&](const std::string_view tag) {
        return line_with_identifier.starts_with(tag);
    });
}
}  // namespace analyser::metric::metric_impl
