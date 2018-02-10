#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <rf/DepthImage.hpp>

using namespace cv;

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cout << " Usage: display_image ImageToLoadAndDisplay" << std::endl;
    return -1;
  }

  Mat image;
  image = imread(argv[1], CV_LOAD_IMAGE_ANYDEPTH | CV_LOAD_IMAGE_ANYCOLOR);
  image.convertTo(image, CV_32F);

  rf::DepthImage<81, 86> di{ image.begin<float>(), image.end<float>() };
  std::cout << di << std::endl;

  if (!image.data) {
    std::cout << "Could not open or find the image" << std::endl;
    return -1;
  }

  namedWindow("Display window", WINDOW_AUTOSIZE);
  imshow("Display window", image);

  waitKey(0);
  return 0;
}
