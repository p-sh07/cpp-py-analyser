#include "metric_impl/code_lines_count.hpp"

#include <gtest/gtest.h>

namespace analyser::metric::metric_impl {
using std::literals::operator""s;

TEST(LinesCountTests, EmptyFile) {
    const file::File file_ {"empty.py"};
    EXPECT_NO_THROW(std::unique_ptr<IMetric> metric_ptr_ = std::make_unique<CodeLinesCountMetric>());
}

// Test fixture to get function from file, list files in params
class LinesCount : public testing::TestWithParam<std::pair<std::string, int>> {
protected:
    const MetricResult::ValueType get_lines_count_for_func(size_t func_num = 0) const {
        function::FunctionExtractor extractor;
        const function::Function func = extractor.Get(file_).at(func_num);

        return metric_ptr_->Calculate(func).value;
    }
private:
    const file::File file_ {GetParam().first};
    std::unique_ptr<IMetric> metric_ptr_ = std::make_unique<CodeLinesCountMetric>();
};

TEST_P(LinesCount, CheckCorrectLineCount) {
    EXPECT_EQ(get_lines_count_for_func(), GetParam().second);
}

INSTANTIATE_TEST_SUITE_P(
    //Filename, correct line count
    LinesCountTests, LinesCount, ::testing::Values(
        std::make_pair("comments.py"s, 4),
        std::make_pair("many_lines.py"s, 11),
        std::make_pair("nested_if.py"s, 11)
    ));
}  // namespace analyser::metric::metric_impl
