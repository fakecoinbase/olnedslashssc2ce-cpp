#include <catch.hpp>
#include <deribit/parser.hpp>

using namespace ssc2ce;

TEST_CASE("Deribit parser, wrong messages", "[deribit-wrong]")
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

TEST_CASE("Deribit parser, book", "[deribit-book]")
{
  DeribitParser parser{};
  SECTION("Snapshot")
  {
    const char *book_snapshot = R"({"jsonrpc":"2.0","method":"subscription","params":{"channel":"book.ETH-PERPETUAL.raw",
    "data":{"timestamp":1592049683328,"instrument_name":"ETH-PERPETUAL","change_id":600242265,
    "bids":[["new",235.3,26441.0],["new",235.25,5882.0],["new",235.2,15126.0]],
    "asks":[["new",235.4,2120.0],["new",235.45,15892.0],["new",235.5,34249.0]]}}})";
    REQUIRE(parser.parse(book_snapshot));

    REQUIRE(parser.get_book("ETH-PERPETUAL").top_ask_price() == 235.4);

    REQUIRE(parser.get_book("ETH-PERPETUAL").top_bid_price() == 235.3);

    const char *book_update = R"({"jsonrpc":"2.0","method":"subscription","params":{"channel":"book.ETH-PERPETUAL.raw",
    "data":{"timestamp":1592049685338,"prev_change_id":600242265,"instrument_name":"ETH-PERPETUAL","change_id":600242267,
    "bids":[["new",235.4,111.0]],"asks":[["delete",235.4],["new",235.46,236066.0]]}}})";
    REQUIRE(parser.parse(book_update));

    REQUIRE(parser.get_book("ETH-PERPETUAL").top_bid_price() == 235.4);
    REQUIRE(parser.get_book("ETH-PERPETUAL").top_ask_price() == 235.45);
  }
}