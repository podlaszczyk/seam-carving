#include <opencv2/highgui.hpp>

#include "seam_carving.h"

int main(int, char** argv) {
  const auto in = cv::imread(argv[1]);
  const auto out = seamCarving(in, cv::Size(640, 480));
  cv::imwrite("./output.png", out);
  return 0;
}
