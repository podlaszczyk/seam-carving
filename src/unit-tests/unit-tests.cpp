/*
 * Copyright (C) Quick Turn Studio GmbH - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#include <catch2/catch.hpp>
#include <opencv2/viz/types.hpp>
#include "seam_carving.h"

using namespace cv;

TEST_CASE("Hello world with castle image, image is loaded correctly")
{
    const auto image = imread("/workdir/examples/castle.jpg");
    const auto size = Size(600, 600);
    const auto seamCarved = seamCarving(image, size);

    REQUIRE(seamCarved.cols == 1428);
    REQUIRE(seamCarved.rows == 968);
}

TEST_CASE("Grey")
{
    const auto image = imread("/workdir/examples/castle.jpg");
    fs::path path{"/workdir/examples/castle-grey.jpg"};

    grey(image, path);
}

TEST_CASE("Edges")
{
    const auto image = imread("/workdir/examples/castle.jpg");
    fs::path path{"/workdir/examples/castle-edges.jpg"};

    edges(image, path);
}

TEST_CASE("SobelX")
{
    const auto image = imread("/workdir/examples/castle.jpg");
    fs::path path{"/workdir/examples/castle-sobel-x.jpg"};

    sobelX(image, path);
}

TEST_CASE("SobelY")
{
    const auto image = imread("/workdir/examples/castle.jpg");
    fs::path path{"/workdir/examples/castle-sobel-y.jpg"};

    sobelY(image, path);
}

TEST_CASE("SobelXY")
{
    const auto image = imread("/workdir/examples/dali.jpg");
    fs::path path{"/workdir/examples/dali-sobel-x-y2.jpg"};

    const auto output = sobelXY(image);
    imwrite(path, output);
}

TEST_CASE("Minimal Energy")
{
    // clang-format off
    int data[36] = {
                    255 * 0.1, 255 * 0.8, 255 * 0.8, 255 * 0.3, 255 * 0.5, 255 * 0.4,
                    255 * 0.7, 255 * 0.8, 255 * 0.1, 255 * 0.0, 255 * 0.8, 255 * 0.4,
                    255 * 0.8, 255 * 0.0, 255 * 0.4, 255 * 0.7, 255 * 0.2, 255 * 0.9,
                    255 * 0.9, 255 * 0.0, 255 * 0.0, 255 * 0.5, 255 * 0.9, 255 * 0.4,
                    255 * 0.2, 255 * 0.4, 255 * 0.0, 255 * 0.2, 255 * 0.4, 255 * 0.5,
                    255 * 0.2, 255 * 0.4, 255 * 0.2, 255 * 0.5, 255 * 0.3, 255 * 0.0};
    cv::Mat grey(6, 6, CV_32F, data);

    int minData[36] = {
                    255 * 1.0, 255 * 1.1, 255 * 1.1, 255 * 0.6, 255 * 1.1, 255 * 1.7,
                    255 * 0.9, 255 * 1.0, 255 * 0.3, 255 * 0.6, 255 * 1.7, 255 * 1.3,
                    255 * 1.0, 255 * 0.2, 255 * 0.6, 255 * 0.9, 255 * 0.9, 255 * 1.7,
                    255 * 1.3, 255 * 0.2, 255 * 0.2, 255 * 0.7, 255 * 1.3, 255 * 0.8,
                    255 * 0.4, 255 * 0.6, 255 * 0.2, 255 * 0.4, 255 * 0.4, 255 * 0.5,
                    255 * 0.2, 255 * 0.4, 255 * 0.2, 255 * 0.5, 255 * 0.3, 255 * 0.0};

    float expectedArrowsData[36] = {
                    0, 1, 0, -1, -1, 0,
                    1, 0, -1, -1, -1, -1,
                    1, 0, -1, -1, -1, 0,
                    0, 1, 0, -1, -1, -1,
                    0, -1, 0, -1, 1, 0};

    // clang-format on

    cv::Mat minEnergy(6, 6, CV_32S);
    cv::Mat expectedMinEnergy(6, 6, CV_32S, minData);
    cv::Mat expectedArrows(5, 6, CV_32F, expectedArrowsData);
    const auto arrows = minimalEnergyToBottom<int, int>(grey, minEnergy);

    cv::Ptr<cv::Formatter> fmt = cv::Formatter::get(cv::Formatter::FMT_DEFAULT);
    fmt->set32fPrecision(1);

    std::cout << fmt->format(expectedMinEnergy) << "\n\n";
    std::cout << fmt->format(minEnergy);

    bool areIdentical = false;
    const auto norm = cv::norm(expectedMinEnergy, minEnergy, cv::NORM_L1);
    if (fabs(0.0 - norm) < 0.1)
    {
        areIdentical = true;
    }
    REQUIRE(areIdentical);

    std::cout << "\n\nArrows Expected\n";
    std::cout << fmt->format(expectedArrows) << "\n\n Arrows \n";
    std::cout << fmt->format(arrows);
    bool areArrowsIdentical = false;
    const auto normArrows = cv::norm(expectedArrows, arrows, cv::NORM_L1);
    if (fabs(0.0 - normArrows) < 0.1)
    {
        areArrowsIdentical = true;
    }
    REQUIRE(areArrowsIdentical);
}

TEST_CASE("Minimal Energy For Castle")
{
    const auto image = imread("/workdir/examples/dali.jpg");
    const auto output = sobelXY(image);

    fs::path path{"/workdir/examples/dali-minimal-energy-sobel-x-y.png"};

    const auto type = output.type();
    (void)type;

    //    cv::Mat minEnergy;
    //    output.convertTo(minEnergy, CV_16U, 255);
    cv::Mat minEnergy(output.rows, output.cols, CV_8U);
    minimalEnergyToBottom<uchar, uchar>(output, minEnergy);

    //    cv::Mat res;
    //    minEnergy.convertTo(res, CV_16U, 1, 255*255/2);

    imwrite(path, minEnergy);
}

// TEST_CASE("Draw pink rectangle on Castle")
//{
//    auto castle = imread("/workdir/examples/castle.jpg");
//    fs::path path{"/workdir/examples/castle-pink.jpg"};
//
//    // rgb(255,105,180) pink
//    viz::Color color(255, 105, 180);
//    for (int i = 200; i < 400; ++i)
//    {
//        for (int j = 100; j < 200; ++j)
//        {
//            Vec3b& color = castle.at<Vec3b>(Point(i, j));
//            color[0] = 255;
//            color[1] = 105;
//            color[2] = 180;
//        }
//    }
//
//    imwrite(path, castle);
//}
//
//
// TEST_CASE("Draw one red curve with least Energy")
//{
//    auto castle = imread("/workdir/examples/castle.jpg");
//    const auto output = sobelXY(castle);
//    fs::path path{"/workdir/examples/castle-red-least-energy-curve.png"};
//
//    //    const auto type = grey.type();
//    cv::Mat minEnergy(output.rows, output.cols, CV_16U);
//    const auto arrows = minimalEnergyToBottom<uchar, ushort>(output, minEnergy);
//
//    drawLeastEnergyCurve(castle, minEnergy, arrows);
//    imwrite(path, castle);
//}
//
// TEST_CASE("Draw 20 red curves with least Energy")
//{
//    auto castle = imread("/workdir/examples/castle.jpg");
//    const auto output = sobelXY(castle);
//    fs::path path{"/workdir/examples/castle-red-least-20-energy-curves.png"};
//
//    //    const auto type = grey.type();
//    cv::Mat minEnergy(output.rows, output.cols, CV_16U);
//    const auto arrows = minimalEnergyToBottom<uchar, ushort>(output, minEnergy);
//
//    drawLeastEnergyCurve(castle, minEnergy, arrows, 20);
//    imwrite(path, castle);
//}
//
// TEST_CASE("Draw 100 red curves with least Energy")
//{
//    auto castle = imread("/workdir/examples/castle.jpg");
//    const auto output = sobelXY(castle);
//    fs::path path{"/workdir/examples/castle-red-least-100-energy-curves.png"};
//
//    //    const auto type = grey.type();
//    cv::Mat minEnergy(output.rows, output.cols, CV_16U);
//    const auto arrows = minimalEnergyToBottom<uchar, ushort>(output, minEnergy);
//
//    drawLeastEnergyCurve(castle, minEnergy, arrows, 100);
//    imwrite(path, castle);
//}
//
// TEST_CASE("Draw 400 red curves with least Energy")
//{
//    auto castle = imread("/workdir/examples/castle.jpg");
//    const auto output = sobelXY(castle);
//    fs::path path{"/workdir/examples/castle-red-least-400-energy-curves.png"};
//
//    //    const auto type = grey.type();
//    cv::Mat minEnergy(output.rows, output.cols, CV_16U);
//    const auto arrows = minimalEnergyToBottom<uchar, ushort>(output, minEnergy);
//
//    drawLeastEnergyCurve(castle, minEnergy, arrows, 400);
//    imwrite(path, castle);
//}
//
// TEST_CASE("Remove one least important red curve from castle")
//{
//    auto castle = imread("/workdir/examples/castle.jpg");
//    const auto grey = imread("/workdir/examples/castle-minimal-energy-sobel-x-y.jpg", IMREAD_GRAYSCALE);
//    fs::path path{"/workdir/examples/castle-shrinked-1.jpg"};
//
//    const auto type = grey.type();
//    cv::Mat minEnergy(grey.rows, grey.cols, type);
//    auto arrows = minimalEnergyToBottom<uchar>(grey, minEnergy);
//    const auto shrinkedCastle = removeEnergyCurve(castle, minEnergy, arrows, 1);
//    imwrite(path, shrinkedCastle);
//}
//
// TEST_CASE("Remove 2 least important red curve from castle")
//{
//    auto castle = imread("/workdir/examples/castle.jpg");
//    const auto grey = imread("/workdir/examples/castle-minimal-energy-sobel-x-y.jpg", IMREAD_GRAYSCALE);
//    fs::path path{"/workdir/examples/castle-shrinked-2.jpg"};
//
//    const auto type = grey.type();
//    cv::Mat minEnergy(grey.rows, grey.cols, type);
//    auto arrows = minimalEnergyToBottom<uchar>(grey, minEnergy);
//    const auto shrinkedCastle = removeEnergyCurve(castle, minEnergy, arrows, 2);
//    imwrite(path, shrinkedCastle);
//}
//
// TEST_CASE("Remove 100 least important red curve from castle")
//{
//    auto castle = imread("/workdir/examples/castle.jpg");
//    const auto grey = imread("/workdir/examples/castle-minimal-energy-sobel-x-y.jpg", IMREAD_GRAYSCALE);
//    fs::path path{"/workdir/examples/castle-shrinked-100.jpg"};
//
//    const auto type = grey.type();
//    cv::Mat minEnergy(grey.rows, grey.cols, type);
//    auto arrows = minimalEnergyToBottom<uchar>(grey, minEnergy);
//    const auto shrinkedCastle = removeEnergyCurve(castle, minEnergy, arrows, 100);
//    imwrite(path, shrinkedCastle);
//}

// TEST_CASE("Remove 200 least important red curve from castle")
//{
//    auto castle = imread("/workdir/examples/castle.jpg");
//    const auto sobelOutput = sobelXY(castle);
////    const auto grey = imread("/workdir/examples/castle-minimal-energy-sobel-x-y.jpg", IMREAD_GRAYSCALE);
//    fs::path path{"/workdir/examples/castle-shrinked-200.jpg"};
//
//    cv::Mat minEnergy(sobelOutput.rows, sobelOutput.cols, CV_16S);
//    auto arrows = minimalEnergyToBottom<uchar, short>(sobelOutput, minEnergy);
//    const auto shrinkedCastle = removeEnergyCurve(castle, minEnergy, arrows, 200);
//    imwrite(path, shrinkedCastle);
//}