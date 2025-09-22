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

struct CountParametersMetric final: public IMetric {
protected:
    MetricResult::ValueType CalculateImpl(const function::Function& f) const override;
    std::string Name() const override;

private:
    static constexpr std::string_view parameter_tag_ = "identifier"sv;
    static constexpr std::string_view body_tag_ ="body"sv;

    static bool NotReachedFunctionBody(std::string_view line);
    static bool MatchParameterTag(std::string_view line);
};

} // namespace analyser::metric::metric_impl
