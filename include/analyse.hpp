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

namespace vws = std::ranges::views;
namespace rgs = std::ranges;

auto AnalyseFunctions(const std::vector<std::string>& files,
                      const analyser::metric::MetricExtractor& metric_extractor) {
    // AnalyseFunctions, которая:
    // принимает список имен файлов и объект MetricExtractor,
    // запускает создание структур File на основе имен файлов,
    // для каждого файла с помощью FunctionExtractor выделяет список объектов Function,
    // объединяет списки объектов Function в один список,
    // выделяет из каждой функции метрики,
    // возвращает вектор пар из объекта Function и набора результатов вычисления метрик MetricResult;

    //Analyse first function of first file
    auto functions = files
    | vws::transform([&](const std::string& name) { return file::File{name}; })
    | vws::transform([](auto&& range) {
        function::FunctionExtractor extractor;
        return extractor.Get(range);
    });


}

auto SplitByClasses(const auto& analysis) {
    // здесь ваш код
}

auto SplitByFiles(const auto& analysis) {
    // здесь ваш код
}

void AccumulateFunctionAnalysis(
    const auto& analysis, const analyser::metric_accumulator::MetricsAccumulator& accumulator) {
    // здесь ваш код
}

} // namespace analyser
