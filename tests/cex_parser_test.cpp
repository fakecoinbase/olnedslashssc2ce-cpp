// Copyright Oleg Nedbaylo 2020.
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE
// or copy at http://www.boost.org/LICENSE_1_0.txt

#include <catch.hpp>
#include <cex/parser.hpp>

using namespace ssc2ce;

TEST_CASE("CEX parser, book", "[cex-book]")
{
  CexParser parser{};

  const char *book_snapshot = R"({"e":"order-book-subscribe",
    "data":{"timestamp":1592738201,
    "bids":[[8355.0,0.80000000],[8354.9,0.08562700],[8354.6,0.26770000]],
    "asks":[[8384.3,0.80000000],[8384.4,0.10000000],[8392.4,1.00000000]],
    "pair":"BTC:EUR","id":346101544,"sell_total":"176.22612139","buy_total":"1330266.42"},
    "oid":"book-1", "ok":"ok"})";

  REQUIRE(parser.parse(book_snapshot));

  auto book = parser.get_book("BTC:EUR");

  REQUIRE(book->top_ask_price() == 8384.3);
  REQUIRE(book->top_bid_price() == 8355.0);

  {
    const char *book_update = R"({"e":"md_update","data":{"id":346101545,"pair":"BTC:EUR","time":1592738202189,
      "bids":[[8355.1,0.08562700],[8354.9,0.00000000]],"asks":[]}})";
    REQUIRE(parser.parse(book_update));
  }

  REQUIRE(book->top_ask_price() == 8384.3);
  REQUIRE(book->top_bid_price() == 8355.1);

  {
    const char *book_update = R"({"e":"md_update","data":{"id":346101546,"pair":"BTC:EUR","time":1592738202288,
      "bids":[[8355,0.00000000]],"asks":[]}})";

    REQUIRE(parser.parse(book_update));
  }

  REQUIRE(book->top_ask_price() == 8384.3);
  REQUIRE(book->top_bid_price() == 8355.1);

  {
    const char *book_update = R"({"e":"md_update","data":{"id":346101547,"pair":"BTC:EUR","time":1592738202313,
      "bids":[[8355.2,0.80000000]],"asks":[]}})";

    REQUIRE(parser.parse(book_update));
  }

  REQUIRE(book->top_ask_price() == 8384.3);
  REQUIRE(book->top_bid_price() == 8355.2);

  {
    const char *book_update = R"({"e":"md_update","data":{"id":346101548,"pair":"BTC:EUR","time":1592738206257,
      "bids":[],"asks":[[8374.9,0.26770000],[8412.2,0.00000000]]}})";

    REQUIRE(parser.parse(book_update));
  }

  REQUIRE(book->top_ask_price() == 8374.9);
  REQUIRE(book->top_bid_price() == 8355.2);
}