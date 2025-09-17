#include "metric_impl/parameters_count.hpp"

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
#include <string_view>

namespace analyser::metric::metric_impl {
namespace rgs = std::ranges;
namespace vws = std::views;

MetricResult::ValueType CountParametersMetric::CalculateImpl(const function::Function& f) const {
    //TODO:
    return 0;
}

std::string CountParametersMetric::Name() const {
    return "Parameters Count";
}



}  // namespace analyser::metric::metric_impl
