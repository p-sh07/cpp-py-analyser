#include "metric_accumulator.hpp"

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

namespace analyser::metric_accumulator {

//Accumulates results for the vector of metrics
void MetricsAccumulator::AccumulateNextFunctionResults(const std::vector<metric::MetricResult>& metric_results) const {
    rg::for_each(metric_results, [&](const auto& result) { accumulators_.at(result.metric_name)->Accumulate(result); });
}

void MetricsAccumulator::ResetAccumulators() {
    rg::for_each(accumulators_, [&](const auto& acc) { acc.second->Reset(); });
}

}  // namespace analyser::metric_accumulator
