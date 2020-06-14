#include <catch.hpp>
#include <deribit/parser.hpp>

using namespace ssc2ce;

TEST_CASE("Deribit parser", "[deribit-wrong]")
{
  DeribitParser parser{};
  SECTION("empty string")
  {
    REQUIRE_FALSE(parser.parse(""));
    REQUIRE(parser.last_error_msg() == "Empty string.");
  }

  SECTION("wrong format")
  {
    REQUIRE_FALSE(parser.parse("["));
    REQUIRE(parser.last_error_msg() == "Unable to parse the message, probably the wrong JSON format.");
  }

  SECTION("Unknown message format")
  {
    REQUIRE_FALSE(parser.parse("{}"));
    REQUIRE(parser.last_error_msg() == "DeribitParser Unknown message format: {}");
  }
}
