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

struct NamingStyleMetric: IMetric {
    std::string Name() const override;

protected:
    MetricResult::ValueType CalculateImpl(const function::Function& f) const override;
};

} // namespace analyser::metric::metric_impl
