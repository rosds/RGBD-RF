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
  return 0;
}
