#pragma once

#include <opencv2/core/mat.hpp>

cv::Mat seamCarving(const cv::Mat& image, const cv::Size& out_size);
