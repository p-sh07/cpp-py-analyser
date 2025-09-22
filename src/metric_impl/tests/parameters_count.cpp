#include "metric_impl/parameters_count.hpp"

#include <gtest/gtest.h>

namespace analyser::metric::metric_impl {

using std::literals::operator""s;

// Test fixture to get function from file, list files in params
class ParamCount : public testing::TestWithParam<std::pair<std::string, int>> {
protected:
    const MetricResult::ValueType get_param_count_for_func(size_t func_num = 0) const {
        function::FunctionExtractor extractor;
        const function::Function func = extractor.Get(file_).at(func_num);

        return metric_ptr_->Calculate(func).value;
    }
private:
    const file::File file_ {GetParam().first};
    std::unique_ptr<IMetric> metric_ptr_ = std::make_unique<CountParametersMetric>();
};

TEST_P(ParamCount, CheckCorrectLineCount) {
    EXPECT_EQ(get_param_count_for_func(), GetParam().second);
}

INSTANTIATE_TEST_SUITE_P(
    //Filename, correct line count
    LinesCountTests, ParamCount, ::testing::Values(
        std::make_pair("comments.py"s, 3),
        std::make_pair("exceptions.py"s, 0),
        std::make_pair("many_parameters.py"s, 5),
        std::make_pair("match_case.py"s, 1),
        std::make_pair("nested_if.py"s, 2)
    )); //etc, if needed



}  // namespace analyser::metric::metric_impl
