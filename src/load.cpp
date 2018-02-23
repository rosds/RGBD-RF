#include <iostream>
#include <rf/Image.hpp>

using namespace cv;

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cout << " Usage: display_image ImageToLoadAndDisplay" << std::endl;
    return -1;
  }

  rf::DepthImage di{};
  di.path(argv[1]);
  std::cout << di(49, 40) << std::endl;
  return 0;
}
