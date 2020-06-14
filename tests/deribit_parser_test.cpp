#include <deribit/parser.hpp>
#include <catch.hpp>

using namespace ssc2ce;

TEST_CASE("test Index", "[tcc-index]")
{
  DeribitParser parser{};
  SECTION("empty string")
  {
    REQUIRE_FALSE(parser.parse(""));
  }
}
