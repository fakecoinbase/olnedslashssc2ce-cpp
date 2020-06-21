// Copyright Oleg Nedbaylo 2020.
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE
// or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once
#include "book_l2.hpp"
#include <common/parser.hpp>
#include <rapidjson/document.h>
#include <string_view>
#include <unordered_map>

namespace ssc2ce {

class CexParser : public Parser {
public:
  CexParser();
  ~CexParser() {}

  bool parse(const char *message) override;

  std::string last_error_msg() const
  {
    return last_error_msg_;
  }

  void reset_error()
  {
    last_error_msg_.clear();
  }

  BookL2 const *get_book(const std::string_view &instrument) override;

private:
  std::string last_error_msg_;
  CexBookL2 &find_or_create_book(const std::string_view &instrument);
  bool parse_book_update(const rapidjson::Value &);
  bool parse_book_setup(const rapidjson::Value &);
  std::unordered_map<std::size_t, CexBookL2> books_;
};

} // namespace ssc2ce