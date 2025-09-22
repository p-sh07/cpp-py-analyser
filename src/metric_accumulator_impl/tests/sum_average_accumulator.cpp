#include "metric_accumulator_impl/sum_average_accumulator.hpp"

#include <gtest/gtest.h>

#include <cmath>

namespace analyser::metric_accumulator::metric_accumulator_impl::test {

using metric::MetricResult;
using metric::MetricResultsVector;
using std::literals::operator""s;

TEST(SumAvgAccumTests, NonFinalisedAccumThrow) {
    auto avg_acc_ = std::make_unique<SumAverageAccumulator>();
    EXPECT_THROW(avg_acc_->Get(), std::runtime_error);
}

TEST(SumAvgAccumTests, ResetAccumThrow) {
    auto avg_acc_ = std::make_unique<SumAverageAccumulator>();
    avg_acc_->Finalize();
    avg_acc_->Reset();
    EXPECT_THROW(avg_acc_->Get(), std::runtime_error);
}

// Test fixture to get function from file, list files in params
class SumAvgAccum : public testing::TestWithParam<std::pair<MetricResultsVector, SumAverageAccumulator::SumAverage>> {
protected:
    SumAverageAccumulator::SumAverage get_result(const MetricResultsVector& metric_results) {
        rg::for_each(metric_results, [&](const auto& metric_result) {
            sum_avg_acc_->Accumulate(metric_result);
        });

        sum_avg_acc_->Finalize();
        return sum_avg_acc_->Get();
    }
private:
    std::unique_ptr<SumAverageAccumulator> sum_avg_acc_ = std::make_unique<SumAverageAccumulator>();
};

TEST_P(SumAvgAccum, CheckCorrectAverage) {
    EXPECT_EQ(get_result(GetParam().first), GetParam().second);
}

INSTANTIATE_TEST_SUITE_P(
    //Filename, correct line count
    SumAvgAccumTests, SumAvgAccum, ::testing::Values(
        std::make_pair( MetricResultsVector{{"some_result", 5}, {"some_result2", 4}, {"some_result3", 3}}, SumAverageAccumulator::SumAverage{12,4}),
        std::make_pair( MetricResultsVector{{"some_result", 0}} , SumAverageAccumulator::SumAverage{0,0}),
        std::make_pair( MetricResultsVector{}, SumAverageAccumulator::SumAverage{0,0})
    ));

}  // namespace analyser::metric_accumulator::metric_accumulator_impl::test
