// Copyright Oleg Nedbaylo 2020.
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE
// or copy at http://www.boost.org/LICENSE_1_0.txt

#include <catch.hpp>
#include <coinbase_pro/parser.hpp>

using namespace ssc2ce;

TEST_CASE("Coinbase parser, wrong messages", "[coinbase-wrong]")
{
  CoinbaseParser parser{};
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
    REQUIRE(parser.last_error_msg() == "CoinbaseParser Unknown message format: {}");
  }
}

TEST_CASE("Coinbase parser, book", "[coinbase-book]")
{
  CoinbaseParser parser{};

  SECTION("Snapshot")
  {
    const char *book_snapshot = R"({"type":"snapshot","product_id":"BTC-EUR","asks":[["8377.45","2899.99000000"],["8377.46","4771.00000000"]],
    "bids":[["8377.43","3009.98800000"],["8377.42","4771.00000000"]]})";
    REQUIRE(parser.parse(book_snapshot));

    auto book = parser.get_book("BTC-EUR");

    REQUIRE(book->top_ask_price() == 8377.45);
    REQUIRE(book->top_bid_price() == 8377.43);

    {
      const char *book_update = R"({"type":"l2update","product_id":"BTC-EUR","changes":[["buy","8377.44","2.04180000"],["sell","8377.45","2.0"]],
    "time":"2020-06-18T10:19:58.975558Z"})";
      REQUIRE(parser.parse(book_update));
    }

    REQUIRE(book->top_ask_price() == 8377.45);
    REQUIRE(book->top_bid_price() == 8377.44);

    {
      const char *book_update = R"({"type":"l2update","product_id":"BTC-EUR","changes":[["buy","8377.44","0"],["sell","8377.44","4.0"]],
    "time":"2020-06-18T10:19:58.981559Z"})";

      REQUIRE(parser.parse(book_update));
    }

    REQUIRE(book->top_ask_price() == 8377.44);
    REQUIRE(book->top_bid_price() == 8377.43);
  }
}