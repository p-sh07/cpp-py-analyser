#include <unistd.h>

#include <algorithm>
#include <array>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <print>
#include <ranges>
#include <sstream>
#include <string>
#include <variant>
#include <vector>

#include "file.hpp"
#include "function.hpp"
#include "metric.hpp"
#include "metric_accumulator.hpp"

namespace analyser {
using AnalysisResult = std::pair<function::Function, metric::MetricResultsVector>;
using AnalysisResultVector = std::vector<AnalysisResult>;

inline AnalysisResultVector AnalyseFunctions(const std::vector<std::string>& files,
                             const metric::MetricExtractor& metric_extractor) {
    return files
        | vw::transform([&](const std::string& name) { return file::File{name}; })
        | vw::transform([](file::File&& file) { //-> 1.make std::vector<Function> for each file
                function::FunctionExtractor extractor;
                return extractor.Get(file);
            })
        | vw::join //-> 2.join into single vector of functions
        | vw::transform([&](const auto& func) {
                return std::make_pair(func, metric_extractor.Get(func));
            })
        | rg::to<std::vector>(); //-> 3.vector of pairs func/result
}

auto SplitByClasses(const AnalysisResultVector& analysis) {
    return analysis
        | vw::filter([](const auto& result) {
                return result.first.class_name.has_value();
            })
        | vw::chunk_by([](const auto& pair1, const auto& pair2) {
                return pair1.first.class_name.value() == pair2.first.class_name.value();
            })
        | rg::to<std::vector<std::vector<AnalysisResult>>>();
}

auto SplitByFiles(const AnalysisResultVector& analysis) {
    return analysis
        | vw::chunk_by([](const auto& pair1, const auto& pair2) {
            return pair1.first.filename == pair2.first.filename;
            })
        | rg::to<std::vector<std::vector<AnalysisResult>>>();
}

inline void AccumulateFunctionAnalysis(
    const AnalysisResultVector& results, const metric_accumulator::MetricsAccumulator& accumulator) {
    rg::for_each(results, [&](const auto& result_pair) {
        accumulator.AccumulateNextFunctionResults(result_pair.second);
    });
} // &AnalysisResult::second); ?

} // namespace analyser

// Formatter для Analysis Result
template <>
struct std::formatter<analyser::AnalysisResult, char> {
    template<typename FormatContext>
    auto format(const analyser::AnalysisResult& result, FormatContext& fc) const {
        const auto& [func, metrics] = result;

        //form & print function header
        std::string header = func.filename;
        header += func.class_name.has_value() ? "::" + func.class_name.value() : "";
        header += "::" + func.name;

        return format_to(fc.out(), "{}:\n{}", header, metrics);
    }
    constexpr auto parse(format_parse_context &ctx) { return ctx.begin(); }
};

// Formatter для Analysis Result Vector
template <>
struct std::formatter<std::vector<analyser::AnalysisResult>, char> {
    template<typename FormatContext>
    auto format(const std::vector<analyser::AnalysisResult>& results, FormatContext& fc) const {
        rg::for_each(results, [&](const auto& result) {
            format_to(fc.out(), "{}\n", result);
        });
        return fc.out();
    }
    constexpr auto parse(format_parse_context &ctx) { return ctx.begin(); }
};