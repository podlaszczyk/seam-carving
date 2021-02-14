/*
 * Copyright (C) Quick Turn Studio GmbH - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#include <catch2/catch.hpp>
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
    const auto image = imread("/workdir/examples/castle.jpg");
    fs::path path{"/workdir/examples/castle-sobel-x-y.jpg"};

    sobelXY(image, path);
}

TEST_CASE("Minimal Energy")
{
    // clang-format off
    float data[36] = {
                    0.1, 0.8, 0.8, 0.3, 0.5, 0.4,
                    0.7, 0.8, 0.1, 0.0, 0.8, 0.4,
                    0.8, 0.0, 0.4, 0.7, 0.2, 0.9,
                    0.9, 0.0, 0.0, 0.5, 0.9, 0.4,
                    0.2, 0.4, 0.0, 0.2, 0.4, 0.5,
                    0.2, 0.4, 0.2, 0.5, 0.3, 0.0};
    cv::Mat grey(6, 6, CV_32F, data);

    float minData[36] = {
                    1.0, 1.1, 1.1, 0.6, 1.1, 1.7,
                    0.9, 1.0, 0.3, 0.6, 1.7, 1.3,
                    1.0, 0.2, 0.6, 0.9, 0.9, 1.7,
                    1.3, 0.2, 0.2, 0.7, 1.3, 0.8,
                    0.4, 0.6, 0.2, 0.4, 0.4, 0.5,
                    0.2, 0.4, 0.2, 0.5, 0.3, 0.0};
    // clang-format on

    cv::Mat minEnergy(6, 6, CV_32F);
    cv::Mat expectedMinEnergy(6, 6, CV_32F, minData);
    minimalEnergyToBottom<float>(grey, minEnergy);

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
}

TEST_CASE("Minimal Energy For Castle")
{
    const auto grey = imread("/workdir/examples/castle-sobel-x-y.jpg", IMREAD_GRAYSCALE);
    fs::path path{"/workdir/examples/castle-minimal-energy.jpg"};

    const auto type = grey.type();
    //    cvtColor(image, greyMat, cv::COLOR_BGR2GRAY);

    cv::Mat minEnergy(grey.rows, grey.cols, type);
    minimalEnergyToBottom<uchar>(grey, minEnergy);
    imwrite(path, minEnergy);
}