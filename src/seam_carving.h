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
cv::Mat sobelXY(const cv::Mat& image, fs::path out);
void printImage(const cv::Mat& image);

template <typename type>
cv::Mat minimalEnergyToBottom(const cv::Mat& Energy, cv::Mat& MinEnergy)
{
    cv::Mat arrows(Energy.rows - 1, Energy.cols, CV_32F);

    if (Energy.channels() != 1)
    {
        throw std::logic_error("too many channels");
    }
    // copy bottom row
    Energy.row(Energy.rows - 1).copyTo(MinEnergy.row(MinEnergy.rows - 1));

    cv::Ptr<cv::Formatter> fmt = cv::Formatter::get(cv::Formatter::FMT_DEFAULT);
    fmt->set32fPrecision(2);

    const auto penultimate = Energy.rows - 2;
    const auto cols = MinEnergy.cols;
    const auto lastColumn = MinEnergy.cols - 1;

    // process bottom up from penultimate
    for (int r = penultimate; r >= 0; r--)
    {
        for (int c = 0; c < cols; ++c)
        {
            std::vector<type> children;
            if (c != 0 && c != lastColumn)
            {
                children = {MinEnergy.at<type>(r + 1, c - 1), MinEnergy.at<type>(r + 1, c),
                            MinEnergy.at<type>(r + 1, c + 1)};
            }
            else if (c == 0)
            {
                children = {MinEnergy.at<type>(r + 1, c), MinEnergy.at<type>(r + 1, c + 1)};
            }
            else
            {
                children = {MinEnergy.at<type>(r + 1, c - 1), MinEnergy.at<type>(r + 1, c)};
            }

            const auto minimalChild = std::min_element(children.cbegin(), children.cend());
            MinEnergy.at<type>(r, c) = Energy.at<type>(r, c) + *minimalChild;

            if (c == 0)
            {
                if (minimalChild == children.cbegin())
                {
                    arrows.at<float>(r, c) = 0;
                }
                else
                {
                    arrows.at<float>(r, c) = 1;
                }
            }
            else if (c == lastColumn)
            {
                if (minimalChild == children.cbegin())
                {
                    arrows.at<float>(r, c) = -1;
                }
                else
                {
                    arrows.at<float>(r, c) = 0;
                }
            }
            else
            {
                if (minimalChild == children.cbegin())
                {
                    arrows.at<float>(r, c) = -1;
                }
                else if (minimalChild == children.begin() + 1)
                {
                    arrows.at<float>(r, c) = 0;
                }
                else
                {
                    arrows.at<float>(r, c) = 1;
                }
            }
            //            std::cout << std::dec << "col: " << c << " row: " << r << " min: " << minimalChild << " from
            //            "; for (const auto child : children)
            //            {
            //                std::cout << child << " ";
            //            }
            //            std::cout << "\n";
        }
        //        std::cout << "\n";
    }
    return arrows;
}
