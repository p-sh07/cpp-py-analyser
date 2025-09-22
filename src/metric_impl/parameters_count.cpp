#include "metric_impl/parameters_count.hpp"

#include <functional>
#include <ranges>
#include <string>
#include <string_view>

namespace analyser::metric::metric_impl {

MetricResult::ValueType CountParametersMetric::CalculateImpl(const function::Function& f) const {
    return static_cast<MetricResult::ValueType>(
        rg::distance(
            utils::GetStringViewLines(f.ast)
            | vw::take_while(NotReachedFunctionBody)
            | vw::filter(MatchParameterTag)
        ) - 1 //subtract function name identifier
    );

}

std::string CountParametersMetric::Name() const {
    return "Parameters Count";
}

bool CountParametersMetric::NotReachedFunctionBody(std::string_view line) {
    return !line.contains(body_tag_);
}

bool CountParametersMetric::MatchParameterTag(std::string_view line) {
    //discard empty lines
    if(line.empty()) {
        return false;
    }
    return line.contains(parameter_tag_);
}
}  // namespace analyser::metric::metric_impl
