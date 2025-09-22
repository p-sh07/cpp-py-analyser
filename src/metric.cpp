#include "metric.hpp"

#include <unistd.h>

#include <algorithm>
#include <any>
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

#include "function.hpp"

namespace analyser::metric {

void MetricExtractor::RegisterMetric(std::unique_ptr<IMetric> metric) {
    metrics_.push_back(std::move(metric));
}

MetricResultsVector MetricExtractor::Get(const function::Function& func) const {
    return metrics_
    | vw::transform([&](const auto& metric_ptr){ return metric_ptr->Calculate(func); })
    | rg::to<std::vector>();

}

}  // namespace analyser::metric
