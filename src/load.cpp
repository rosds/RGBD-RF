#include <iostream>
#include <rf/Image.hpp>

using namespace cv;

int main(int argc, char *argv[]) {
  if (argc != 2) {
    return -1;
  }

  rf::DepthImage di{};
  di.path(argv[1]);
  di.load();
  auto p = di.pixel(49, 40);
  std::cout << di(49, 40) << std::endl;
  std::cout << p.value() << std::endl;

  cv::namedWindow("Display", CV_WINDOW_NORMAL);
  cv::Mat normalized;
  cv::normalize(di, normalized, 1, 0, cv::NORM_MINMAX);
  imshow("Display", normalized);

  waitKey(0);
  return 0;
}
