// Copyright Oleg Nedbaylo 2020.
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE
// or copy at http://www.boost.org/LICENSE_1_0.txt

#include "parser.hpp"
#include <cstring>
#include <fmt/format.h>
#include <iostream>
#include <rapidjson/document.h>

namespace ssc2ce {

CexParser::CexParser()
{
}

bool CexParser::parse(const char *message)
{

  if (message[0] == char(0)) {
    last_error_msg_ = "Empty string.";
    return false;
  }

  last_error_msg_.clear();

  using namespace rapidjson;
  rapidjson::Document doc;
  doc.Parse(message);
  if (doc.IsNull()) {
    last_error_msg_ = "Unable to parse the message, probably the wrong JSON format.";
    return false;
  }

  bool processed = false;

  if (doc.HasMember("e")) {
    const char *message_type = doc["e"].GetString();

    switch (message_type[0]) {
    case 'a':
      // auth
      break;
    case 'b':
      // balance

      break;
    case 'd':
      // disconnecting
      break;
    case 'g': {
      // get-balance
    } break;
    case 'm':
      // md_update
      return parse_book_update(doc);
      break;
    case 'o': {
      const auto size = std::strlen(message_type);
      // constexpr auto obalance_size = std::strlen("obalance");
      // constexpr auto open_orders_size = std::strlen("open-orders");
      // constexpr auto order_size = std::strlen("order");
      constexpr auto order_book_subscribe_size = std::strlen("order-book-subscribe");
      // constexpr auto order_book_unsubscribe_size = std::strlen("order-book-unsubscribe");

      switch (size) {
      // case obalance_size:
      //   // obalance
      //   break;
      // // open-orders
      // case open_orders_size:
      //   break;
      // case order_size:
      //   // order
      //   break;
      case order_book_subscribe_size:
        // order-book-subscribe
        return parse_book_setup(doc);
      // case order_book_unsubscribe_size:
      //   // order-book-unsubscribe
      //   break;
      default:
        break;
      }
    } break;
    default:
      break;
    }

    if (!processed) {
      last_error_msg_ = fmt::format("CexParser Unsupported: {} in message: {}", message_type, message);
    }
  } else {
    last_error_msg_ = fmt::format("CexParser Unknown message format: {}", message);
  }

  return processed;
}

bool CexParser::parse_book_setup(const rapidjson::Value &doc)
{
  if (doc.HasMember("ok")) {
    const char *message_type = doc["ok"].GetString();
    if (std::strcmp(message_type, "ok") != 0) {
      return false;
    }
  }

  const auto &data = doc["data"];
  auto &book = find_or_create_book(data["pair"].GetString()); //, pair.Size()
  book.clear();
  book.set_last_id(data["id"].GetInt64());
  book.set_timestamp(data["timestamp"].GetInt64());

  const auto &bids{data["bids"]};
  for (const auto &item : bids.GetArray()) {
    book.add_bid(item[0].GetDouble(), item[1].GetDouble());
  }

  const auto &asks{data["asks"]};
  for (const auto &item : asks.GetArray()) {
    book.add_ask(item[0].GetDouble(), item[1].GetDouble());
  }

  if (on_book_setup_) {
    on_book_setup_(&book);
  }

  return true;
}

bool CexParser::parse_book_update(const rapidjson::Value &doc)
{
  const auto &data = doc["data"];
  auto &book = find_or_create_book(data["pair"].GetString()); //, pair.Size()

  const auto id = data["id"].GetInt64();
  if (id != book.get_last_id() + 1) {
    // TODO
    return false;
  }

  book.set_last_id(id);
  book.set_timestamp(data["time"].GetInt64());

  const auto &bids{data["bids"]};
  for (const auto &item : bids.GetArray()) {
    book.update_bid(item[0].GetDouble(), item[1].GetDouble());
  }

  const auto &asks{data["asks"]};
  for (const auto &item : asks.GetArray()) {
    book.update_ask(item[0].GetDouble(), item[1].GetDouble());
  }

  if (on_book_setup_ && book.valid()) {
    on_book_update_(&book);
  }

  return true;
}

CexBookL2 &CexParser::find_or_create_book(const std::string_view &instrumnet)
{
  const auto key = std::hash<std::string_view>{}(instrumnet);

  if (auto p = books_.find(key); p != books_.end()) {
    return p->second;
  } else {
    auto [x, ok] = books_.emplace(key, CexBookL2(std::string(instrumnet)));
    return x->second;
  }
};

BookL2 const *CexParser::get_book(const std::string_view &instrument)
{
  BookL2 const *book = &find_or_create_book(instrument);
  return book;
}

} // namespace ssc2ce