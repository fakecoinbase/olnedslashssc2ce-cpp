// Copyright Oleg Nedbaylo 2020.
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE
// or copy at http://www.boost.org/LICENSE_1_0.txt

#include "parser.hpp"
#include <cstdlib>
#include <fmt/format.h>
#include <iostream>
#include <rapidjson/document.h>

namespace ssc2ce {

CoinbaseParser::CoinbaseParser()
{
}

bool CoinbaseParser::parse(const char *message)
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
  if (doc.HasMember("type")) {
    const char *message_type = doc["type"].GetString();

    switch (message_type[0]) {
    case 'a':
      // activate
      break;
    case 'c':
      // change
      break;
    case 'd':
      // done
      break;
    case 'h':
      // heartbeat
      break;
    case 'l':
      // l2update
      processed = handle_l2update(doc);
    case 'm':
      // match
      break;
    case 'o':
      // open
      break;
    case 'r':
      // received
      break;
    case 's':
      switch (message_type[1]) {
      case 'n':
        processed = handle_snapshot(doc);
      case 't':
        // status
        break;
      default:
        // subscribe
        break;
      }
      break;
    case 't':
      // ticker
      break;
    default:
      break;
    }

    if (!processed) {
      last_error_msg_ = fmt::format("CoinbaseParser Unsupported: {} in message: {}", message_type, message);
    }
  } // namespace ssc2ce
  else {
    last_error_msg_ = fmt::format("CoinbaseParser Unknown message format: {}", message);
  }

  return processed;
} // namespace ssc2ce

bool CoinbaseParser::handle_snapshot(const rapidjson::Value &data)
{
  auto &book = find_or_create_book(data["product_id"].GetString());
  book.clear();

  for (const auto &item : data["bids"].GetArray()) {
    auto price = std::atof(item[0].GetString());
    auto size = std::atof(item[1].GetString());
    book.add_bid(price, size);
  }

  for (const auto &item : data["asks"].GetArray()) {
        auto price = std::atof(item[0].GetString());
    auto size = std::atof(item[1].GetString());
    book.add_ask(price, size);
  }

  if (on_book_setup_)
    on_book_setup_(&book);

  return true;
}

bool CoinbaseParser::handle_l2update(const rapidjson::Value &data)
{
  auto &book = find_or_create_book(data["product_id"].GetString());
  book.set_time(data["time"].GetString());

  for (const auto &item : data["changes"].GetArray()) {
    if (item[0].GetString()[0] == 's') // sell
    {
      const auto price = std::atof(item[1].GetString());
      const auto size = std::atof(item[2].GetString());
      book.update_ask(price, size);

    } else {
      const auto price = std::atof(item[1].GetString());
      const auto size = std::atof(item[2].GetString());
      book.update_bid(price, size);
    }
  }

  if (on_book_update_)
    on_book_update_(&book);

  return true;
}

CoinbaseBookL2 &CoinbaseParser::find_or_create_book(const std::string_view &instrumnet)
{
  const auto key = std::hash<std::string_view>{}(instrumnet);

  if (auto p = books_.find(key); p != books_.end()) {
    return p->second;
  } else {
    auto [x, ok] = books_.emplace(key, CoinbaseBookL2(std::string(instrumnet)));
    return x->second;
  }
};

BookL2 const *CoinbaseParser::get_book(const std::string_view &instrument)
{
  BookL2 const *book = &find_or_create_book(instrument);
  return book;
}

} // namespace ssc2ce