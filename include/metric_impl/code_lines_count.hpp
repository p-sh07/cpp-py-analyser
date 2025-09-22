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

struct CodeLinesCountMetric final: IMetric {
   std::string Name() const override;

protected:
   MetricResult::ValueType CalculateImpl(const function::Function& f) const override;

private:
   static bool IsNotComment(std::string_view ast_output_line);
   static int ParseLineNumber(std::string_view sv);
};

} // namespace analyser::metric::metric_impl
