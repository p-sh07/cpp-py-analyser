#pragma once
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

#include "metric.hpp"

namespace rv = std::ranges::views;
namespace rs = std::ranges;

namespace analyser::metric_accumulator {

struct IAccumulator {
    virtual void Accumulate(const metric::MetricResult& metric_result) = 0;
    virtual void Finalize() = 0;
    virtual void Reset() = 0;
    virtual ~IAccumulator() = default;

protected:
    bool is_finalized = false;
};

struct MetricsAccumulator {
    template <typename Accumulator>
    void RegisterAccumulator(const std::string& metric_name, std::unique_ptr<Accumulator> acc) {
        accumulators_.try_emplace(metric_name, std::move(acc));
    }

    template <typename Accumulator>
    const Accumulator& GetFinalizedAccumulator(const std::string& metric_name) const {
        const auto accumulator = dynamic_cast<Accumulator*>(accumulators_.at(metric_name).get());
        accumulator->Finalize();
        return *accumulator;
    }

    void AccumulateNextFunctionResults(
        const metric::MetricResultsVector& metric_results) const;

    void ResetAccumulators();

private:
    using AccumulatorMap = std::unordered_map<std::string, std::shared_ptr<IAccumulator>>;
    AccumulatorMap accumulators_;
};

} // namespace analyser::metric_accumulator
