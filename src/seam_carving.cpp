#include "seam_carving.h"
#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>

cv::Mat seamCarving(const cv::Mat& image, const cv::Size& out_size)
{
    (void)out_size;

    return image;
}

void grey(const cv::Mat& image, fs::path out)
{
    cv::Mat greyMat;

    cvtColor(image, greyMat, cv::COLOR_BGR2GRAY);
    imwrite(out, greyMat);
}

void edges(const cv::Mat& image, fs::path out)
{
    cv::Mat edges;
    Canny(image, edges, 10, 100);
    imwrite(out, edges);
}

void sobelX(const cv::Mat& image, fs::path out)
{
    cv::Mat greyMat;
    cv::Mat gradX;

    cvtColor(image, greyMat, cv::COLOR_BGR2GRAY);
    Sobel(greyMat, gradX, CV_16S, 1, 0);

    imwrite(out, gradX);
}

void sobelY(const cv::Mat& image, fs::path out)
{
    cv::Mat greyMat;
    cv::Mat gradY;

    cvtColor(image, greyMat, cv::COLOR_BGR2GRAY);
    Sobel(greyMat, gradY, CV_16S, 0, 1);

    imwrite(out, gradY);
}

cv::Mat sobelXY(const cv::Mat& image, fs::path out)
{
    cv::Mat greyMat;
    cv::Mat gradX;
    cv::Mat gradY;
    cv::Mat output;

    cvtColor(image, greyMat, cv::COLOR_BGR2GRAY);
    Sobel(greyMat, gradX, CV_64F, 1, 0);
    Sobel(greyMat, gradY, CV_64F, 0, 1);
    addWeighted(gradX, 0.5, gradY, 0.5, 0.0, output);

    imwrite(out, output);
    return output;
}
