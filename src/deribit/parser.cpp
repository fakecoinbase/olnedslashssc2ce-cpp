// Copyright Oleg Nedbaylo 2020.
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE
// or copy at http://www.boost.org/LICENSE_1_0.txt

#include "parser.hpp"
#include <fmt/core.h>
#include <iostream>
#include <rapidjson/document.h>

namespace ssc2ce {

DeribitParser::DeribitParser()
{
}

bool DeribitParser::parse(const char *message)
{
  if (message[0] == char(0)) {
    last_error_msg_ = "Empty string.";
    return false;
  }

  using namespace rapidjson;
  rapidjson::Document doc;
  doc.Parse(message);
  if (doc.IsNull()) {
    last_error_msg_ = "Unable to parse the message, probably the wrong JSON format.";
    return false;
  }

  bool processed = false;
  if (doc.HasMember("method")) {
    auto method = doc["method"].GetString();
    auto &params = doc["params"];
    if (strcmp(method, "subscription") == 0) {
      auto &data = params["data"];
      auto channel = params["channel"].GetString();

      switch (channel[0]) {
      case 'a': {
        // announcements
      } break;
      case 'b': {
        // book.{instrument_name}.{group}.{depth}.{interval} or
        // book.{instrument_name}.{interval}
      } break;
      case 'c': {
        // chart.trades.{instrument_name}.{resolution}
      } break;
      case 'd': {
        // deribit_price_index.{index_name} or deribit_price_ranking.{index_name}
      } break;
      case 'e': {
        // estimated_expiration_price
      } break;
      case 'm': {
        // markprice.options.{index_name}
      } break;
      case 'p': {
        // perpetual.{instrument_name}.{interval} or
        // platform_state
      } break;
      case 'q': {
        // quote.{instrument_name}
      } break;
      case 't': {
        if (channel[1] == 'i') {
          // ticker.{instrument_name}.{interval} or
        } else {
          // trades.{instrument_name}.{interval} or
          // trades.{kind}.{currency}.{interval}
        }
      } break;
      case 'u': // user.*
      {
        switch (channel[5]) {
        case 'c': {
          // user.changes.{instrument_name}.{interval} or
          // user.changes.{kind}.{currency}.{interval}
        } break;
        case 'o': {
          // user.orders.{instrument_name}.raw or
          // user.orders.{instrument_name}.{interval}
          // user.orders.{kind}.{currency}.raw
          // user.orders.{kind}.{currency}.{interval}

        } break;
        case 'p': {
          // user.portfolio.{currency}
        } break;
        case 't': {
          // user.trades.{instrument_name}.{interval} or
          // user.trades.{kind}.{currency}.{interval}
        } break;
        default:
          break;
        }
      } break;
      default:
        break;
      }

      if (!processed) {
        last_error_msg_ = fmt::format("DeribitParser Unsupported: {} in message: {}", channel, message);
      }
    } else {
      last_error_msg_ = fmt::format("DeribitParser Unknown method: {} in message: {}", method, message);
    }
  } else {
    last_error_msg_ = fmt::format("DeribitParser Unknown message format: {}", message);
  }

  return processed;
}

} // namespace ssc2ce