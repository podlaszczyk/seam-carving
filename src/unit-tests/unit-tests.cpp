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