#include "metric_impl/code_lines_count.hpp"

#include <unistd.h>

#include <algorithm>
#include <charconv>
#include <filesystem>
#include <functional>
#include <ranges>
#include <sstream>
#include <string>
#include <unordered_set>

#include "utils.hpp"

namespace analyser::metric::metric_impl {
using std::literals::operator""sv;
namespace vws = std::views;
namespace rgs = std::ranges;

MetricResult::ValueType CodeLinesCountMetric::CalculateImpl(const function::Function& f) const {
    //construct set from processed elements of range -> return number of unique non-comment non-empty line numbers
    return static_cast<MetricResult::ValueType>(
        std::unordered_set(std::from_range,   //construct set from filtered range
            utils::GetStringViewLines(f.ast)  //split into lines by '\n'
            | vws::filter(IsNotComment)       //filter out lines with comments
            | vws::transform(ParseLineNumber) //transform to range of line numbers
        ).size()                              //get number of unique elements (line numbers)
    );
}

std::string CodeLinesCountMetric::Name() const {
    return "Code Lines Count";
}

bool CodeLinesCountMetric::IsNotComment(std::string_view ast_output_line) {
    return !ast_output_line.contains("comment"sv);
}

int CodeLinesCountMetric::ParseLineNumber(std::string_view sv) {
    return utils::ToInt(rgs::find(sv, '[') + 1, rgs::find(sv, ','));
}

}  // namespace analyser::metric::metric_impl
