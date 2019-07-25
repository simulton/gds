#include <vector>
#include "catch2/catch.hpp"
#include "../test.h"
#include "gpds/serialize.h"
#include "color.h"

TEST_CASE( "Color class" )
{
    // Create a color
    GpdsTest::Color red1;
    red1.name = "Red";
    red1.red = 255;
    red1.green = 0;
    red1.blue = 0;

    // Serialize
    std::string serialized;
    REQUIRE( serialize( serialized, red1, "color" ) );

    // Deserialize
    GpdsTest::Color red2;
    REQUIRE( deserialize( serialized, red2, "color" ) );

    // Ensure that data is the same
    REQUIRE( red1 == red2 );
}
