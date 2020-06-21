// Copyright Oleg Nedbaylo 2020.
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE
// or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once
#include "book_l2.hpp"
#include <functional>
#include <string_view>

namespace ssc2ce {

class Parser {
public:
  using BookEvent = std::function<void(BookL2 *)>;

  virtual ~Parser() {}

  virtual bool parse(const char *message) = 0;
  virtual BookL2 const *get_book(const std::string_view &instrument) = 0;

  void set_on_book_setup(BookEvent handler)
  {
    on_book_setup_ = handler;
  }

  void set_on_book_update(BookEvent handler)
  {
    on_book_update_ = handler;
  }


protected:
  BookEvent on_book_setup_;
  BookEvent on_book_update_;
};

} // namespace ssc2ce