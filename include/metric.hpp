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

#include "function.hpp"
#include "utils.hpp"

using std::literals::operator""sv;
namespace vw = std::views;
namespace rg = std::ranges;

namespace analyser::metric {

struct MetricResult {
    using ValueType = int;
    // using ValueType = std::variant<int, std::string>; // если захотите реализовывать метрику
    // naming style
    std::string metric_name; // Название метрики
    ValueType value;         // Значение метрики
};

struct IMetric {
    virtual ~IMetric() = default;
    MetricResult Calculate(const function::Function& f) const {
        return MetricResult{.metric_name = Name(), .value = CalculateImpl(f)};
    }

    virtual std::string Name() const = 0;

protected:
    virtual MetricResult::ValueType CalculateImpl(const function::Function& f) const = 0;
};

using MetricResultsVector = std::vector<MetricResult>;

struct MetricExtractor {
    void RegisterMetric(std::unique_ptr<IMetric> metric);

    MetricResultsVector Get(const function::Function& func) const;
    std::vector<std::unique_ptr<IMetric>> metrics_;
};

} // namespace analyser::metric

// Formatter для MetricResult
template <>
struct std::formatter<analyser::metric::MetricResult, char> {
    template <typename FormatContext>
    auto format(const analyser::metric::MetricResult& result, FormatContext &fc) const {
        return format_to( fc.out(), "    {}: {}", result.metric_name, result.value);
    }

    constexpr auto parse(format_parse_context &ctx) { return ctx.begin(); }
};

// Formatter для MetricResultVector
template <>
struct std::formatter<analyser::metric::MetricResultsVector, char> {
    template <typename FormatContext>
    auto format(const analyser::metric::MetricResultsVector& result_vec, FormatContext& fc) const {
        rg::for_each(result_vec, [&](const auto& result) {
            format_to( fc.out(), "{}\n", result);
        });
        return fc.out();
    }

    constexpr auto parse(format_parse_context &ctx) { return ctx.begin(); }
};