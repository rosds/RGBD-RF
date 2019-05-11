#include <iostream>
#include <rf/ImageFileLoader.hpp>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        return -1;
    }

    auto depth_image = rf::ImageFileLoader::Load<float>(argv[1]);
    std::cout << depth_image(49, 40).value() << std::endl;

    cv::namedWindow("Display", CV_WINDOW_NORMAL);

    std::vector<float> data = std::move(depth_image);

    auto src_img =
        cv::Mat(depth_image.cols(), depth_image.rows(), CV_32F, data.data());

    cv::Mat normalized;
    cv::normalize(src_img, normalized, 0.0, 1.0, cv::NORM_MINMAX);
    cv::imshow("Display", normalized);

    cv::waitKey(0);
    return 0;
}
