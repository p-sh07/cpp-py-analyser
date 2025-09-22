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

#include "analyse.hpp"
#include "cmd_options.hpp"
#include "file.hpp"
#include "function.hpp"
#include "metric.hpp"
#include "metric_accumulator.hpp"
#include "metric_accumulator_impl/accumulators.hpp"
#include "metric_impl/metrics.hpp"

using namespace analyser;
using metric::metric_impl::CodeLinesCountMetric;
using metric::metric_impl::CyclomaticComplexityMetric;
using metric::metric_impl::CountParametersMetric;
using metric_accumulator::metric_accumulator_impl::AverageAccumulator;
using metric_accumulator::metric_accumulator_impl::SumAverageAccumulator;

int main(int argc, char* argv[]) {
    cmd::ProgramOptions options;

    // Parse program options
    options.Parse(argc, argv);
    auto files = options.GetFiles();

    metric::MetricExtractor metric_extractor;

    // Register metrics
    metric_extractor.RegisterMetric(std::make_unique<CodeLinesCountMetric>());
    metric_extractor.RegisterMetric(std::make_unique<CyclomaticComplexityMetric>());
    metric_extractor.RegisterMetric(std::make_unique<CountParametersMetric>());

    // Run analysis on functions using all registered metrics
    auto analysis_results = AnalyseFunctions(files, metric_extractor);

    // Output result
    std::print("{}", analysis_results);

    // Get metric names
    const auto metric_names = metric_extractor.metrics_
        | vw::transform([](const auto& metric) { return metric->Name(); })
        | rg::to<std::vector<std::string>>();

    metric_accumulator::MetricsAccumulator accumulator;

    // Register Accumulators
    accumulator.RegisterAccumulator(metric_names[0], std::make_unique<SumAverageAccumulator>());
    accumulator.RegisterAccumulator(metric_names[1], std::make_unique<SumAverageAccumulator>());
    accumulator.RegisterAccumulator(metric_names[2], std::make_unique<AverageAccumulator>());

    //lambdas for printing result:
    auto print_accum_header = [](std::string type) {
        std::println("Accumulated Analysis for {}:", type);
    };

    auto print_accum_metrics_and_reset = [&metric_names](metric_accumulator::MetricsAccumulator& acc) {
        const auto [code_lines_sum, code_lines_avg] = acc.GetFinalizedAccumulator<SumAverageAccumulator>(metric_names[0]).Get();
        std::println("    {}: sum = {}, avg = {:.2f}", metric_names[0], code_lines_sum, code_lines_avg);

        const auto [cyclomatic_sum, cyclomatic_avg] = acc.GetFinalizedAccumulator<SumAverageAccumulator>(metric_names[1]).Get();
        std::println("    {}: sum = {}, avg = {:.2f}", metric_names[1], cyclomatic_sum, cyclomatic_avg);

        const auto param_count_avg = acc.GetFinalizedAccumulator<AverageAccumulator>(metric_names[2]).Get();
        std::println("    {}: avg = {:.2f}\n", metric_names[2], param_count_avg);

        acc.ResetAccumulators();
    };


    // Split by files & print
    auto by_file_results = SplitByFiles(analysis_results);
    rg::for_each(by_file_results, [&](const auto& file_result) {
        AccumulateFunctionAnalysis(file_result, accumulator);
        print_accum_header("file " + file_result.at(0).first.filename);
        print_accum_metrics_and_reset(accumulator);

    });

    // Split by classes & print
    auto by_class_results = SplitByClasses(analysis_results);
    rg::for_each(by_class_results, [&](const auto& class_result) {
        AccumulateFunctionAnalysis(class_result, accumulator);
        print_accum_header("class " + class_result.at(0).first.class_name.value());
        print_accum_metrics_and_reset(accumulator);
    });

    // Accumulate & print all analysis results:
    AccumulateFunctionAnalysis(analysis_results, accumulator);
    print_accum_header("all files");
    print_accum_metrics_and_reset(accumulator);

    return 0;
}