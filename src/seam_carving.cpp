#include "seam_carving.h"
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

cv::Mat seamCarving(const cv::Mat& image, const cv::Size& out_size)
{
    (void)out_size;

    return image;
}

void grey(const Mat& image, fs::path out)
{
    cv::Mat greyMat;

    cvtColor(image, greyMat, COLOR_BGR2GRAY);
    imwrite(out, greyMat);
}

void edges(const Mat& image, fs::path out)
{
    Mat edges;
    Canny(image, edges, 10, 100);
    imwrite(out, edges);
}
