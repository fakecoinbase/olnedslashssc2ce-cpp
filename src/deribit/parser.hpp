// Copyright Oleg Nedbaylo 2020.
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE
// or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once
#include <common/parser.hpp>
// #include <functional>
// #include <map>
#include "book_l2.hpp"
#include <rapidjson/document.h>
#include <string_view>

namespace ssc2ce {

class DeribitParser : public Parser {
public:
  DeribitParser();
  ~DeribitParser() {}

  bool parse(const char *message) override;

  std::string last_error_msg() const
  {
    return last_error_msg_;
  }

  void reset_error()
  {
    last_error_msg_.clear();
  }

  BookL2 const *get_book(const std::string_view &instrument);

private:
  std::string last_error_msg_;

//   const BookL2 &get_book_(const std::string_view &instrument)
//   {
//     auto &book = books_[instrument];
//     return dynamic_cast<const BookL2 &>(book);
//   }

  bool parse_book(const char *channel, const rapidjson::Value &data);
  //   using ParseChannel = std::function<bool(const char *)>;
  std::map<std::string_view, DeribitBookL2> books_;
};

} // namespace ssc2ce