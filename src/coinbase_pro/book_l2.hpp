// src/coinbase_pro/book_l2.hpp
// Copyright Oleg Nedbaylo 2020.
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE
// or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once
#include <common/book_l2_map.hpp>

namespace ssc2ce {

class CoinbaseParser;

class CoinbaseBookL2 : public BookL2Map {
public:
  CoinbaseBookL2(const std::string &instrument) : BookL2Map{instrument} {}

  void set_time(const char*) {  ; }

protected:
  std::string last_time_;

  friend class CoinbaseParser;
};

} // namespace ssc2ce
