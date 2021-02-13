#pragma once

#include <experimental/filesystem>
#include <opencv2/core/mat.hpp>
#include <opencv2/opencv.hpp>

namespace fs = std::experimental::filesystem;

cv::Mat seamCarving(const cv::Mat& image, const cv::Size& out_size);
void grey(const cv::Mat& image, fs::path out);
void edges(const cv::Mat& image, fs::path out);
void sobelX(const cv::Mat& image, fs::path out);
void sobelY(const cv::Mat& image, fs::path out);
