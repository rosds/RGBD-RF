#include <rf/LabelImage.h>

#include <gtest/gtest.h>

namespace {

class LabelImageFixture : public ::testing::Test {
   public:
    using Image = rf::Image<int>;
    using LabelImage = rf::LabelImage<Image>;

    void SetUp() override {
        source_image = Image(2, 3, std::vector<int>{0, 1, 2, 3, 4, 5});
        label_image = LabelImage(source_image);
    }

    Image source_image;
    LabelImage label_image;
};

TEST_F(LabelImageFixture, WhenQueryLabels_ThenExpectedLabels) {
    EXPECT_EQ(label_image(0, 0).value(), 0);
    EXPECT_EQ(label_image(0, 1).value(), 1);
    EXPECT_EQ(label_image(0, 2).value(), 2);
    EXPECT_EQ(label_image(1, 0).value(), 3);
    EXPECT_EQ(label_image(1, 1).value(), 4);
    EXPECT_EQ(label_image(1, 2).value(), 5);
}

TEST_F(LabelImageFixture, WhenQueryOutsideImage_ThenNullOpt) {
    EXPECT_EQ(label_image(0, 5), std::nullopt);
    EXPECT_EQ(label_image(-1, 1), std::nullopt);
    EXPECT_EQ(label_image(0, -1), std::nullopt);
    EXPECT_EQ(label_image(9, 2), std::nullopt);
}

}  // namespace
