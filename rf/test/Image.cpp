#include <rf/Image.h>

#include <gtest/gtest.h>
#include <opencv2/core/core.hpp>
#include <random>

namespace {

class RandomFloatImageFixture : public ::testing::Test {
   protected:
    void SetUp() override {
        size_t cols = 32;
        size_t rows = 16;

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> dist(0.0, 1000.0);

        raw = std::vector<float>(cols * rows);
        std::generate(raw.begin(), raw.end(), [&] { return dist(gen); });

        image = rf::Image<float>{rows, cols, raw};
        control_image = cv::Mat(rows, cols, CV_32F, raw.data());

        EXPECT_EQ(image.cols(), control_image.cols);
        EXPECT_EQ(image.rows(), control_image.rows);
    }

    std::vector<float> raw;
    rf::Image<float> image;
    cv::Mat control_image;
};

TEST_F(RandomFloatImageFixture, WhenComparingToControl_ThenEqualValues) {
    for (int col = 0; col < image.cols(); ++col) {
        for (int row = 0; row < image.rows(); ++row) {
            EXPECT_EQ(image(row, col).value(),
                      control_image.at<float>(row, col));
        }
    }
}

TEST_F(RandomFloatImageFixture, WhenAccessingOutsideImage_ThenNullOpt) {
    EXPECT_FALSE(image(100, -100));
    EXPECT_FALSE(image(-100, 400));
    EXPECT_FALSE(image(300, 400));
}

}  // namespace
