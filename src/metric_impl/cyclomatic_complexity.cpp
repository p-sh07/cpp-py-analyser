#include "metric_impl/cyclomatic_complexity.hpp"

#include <algorithm>
#include <array>
#include <ranges>
#include <string>

namespace analyser::metric::metric_impl {

//if/else, elif, while, for, try/catch/finally, match/case, assert, ternary operator.
MetricResult::ValueType CyclomaticComplexityMetric::CalculateImpl(const function::Function& f) const {
    return static_cast<MetricResult::ValueType>(
        rg::distance(
            utils::GetStringViewLines(f.ast, '(') //split into lines by (
            | vw::filter(MatchCycleTag)
        )
    );
}

std::string CyclomaticComplexityMetric::Name() const {
    return "Cyclomatic Complexity";
}

bool CyclomaticComplexityMetric::MatchCycleTag(std::string_view line) {
    //Remove whitespace
    while (!line.empty() && line[0] == ' ') {
        line.remove_prefix(1);
    }
    //discard empty lines
    if(line.empty()) {
        return false;
    }
    return rg::any_of(cycle_tags_, [&](const std::string_view tag) {
        return line.starts_with(tag);
    });
}
}  // namespace analyser::metric::metric_impl
