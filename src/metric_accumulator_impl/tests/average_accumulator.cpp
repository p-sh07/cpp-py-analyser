#include "metric_accumulator_impl/average_accumulator.hpp"

#include <gtest/gtest.h>

#include <cmath>

#include "analyse.hpp"

namespace analyser::metric_accumulator::metric_accumulator_impl::test {

using metric::MetricResult;
using metric::MetricResultsVector;
using std::literals::operator""s;

TEST(AvgAccumTests, NonFinalisedAccumThrow) {
    auto avg_acc_ = std::make_unique<AverageAccumulator>();
    EXPECT_THROW(avg_acc_->Get(), std::runtime_error);
}

TEST(AvgAccumTests, ResetAccumThrow) {
    auto avg_acc_ = std::make_unique<AverageAccumulator>();
    avg_acc_->Finalize();
    avg_acc_->Reset();
    EXPECT_THROW(avg_acc_->Get(), std::runtime_error);
}

// Test fixture to get function from file, list files in params
class AvgAccum : public testing::TestWithParam<std::pair<MetricResultsVector, int>> {
protected:
    double get_result(const MetricResultsVector& metric_results) {
        rg::for_each(metric_results, [&](const auto& metric_result) {
            avg_acc_->Accumulate(metric_result);
        });

        avg_acc_->Finalize();
        return avg_acc_->Get();
    }
private:
    std::unique_ptr<AverageAccumulator> avg_acc_ = std::make_unique<AverageAccumulator>();
};

TEST_P(AvgAccum, CheckCorrectAverage) {
    EXPECT_EQ(get_result(GetParam().first), GetParam().second);
}

INSTANTIATE_TEST_SUITE_P(
    //Filename, correct line count
    AvgAccumTests, AvgAccum, ::testing::Values(
        std::make_pair( MetricResultsVector{{"some_result", 9}, {"some_result2", 1}, {"some_result3", 2}} , 4),
        std::make_pair( MetricResultsVector{{"some_result", 0}} , 0),
        std::make_pair( MetricResultsVector{}, 0)
    ));
}