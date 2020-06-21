// src/cex/book_l2.hpp
// Copyright Oleg Nedbaylo 2020.
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE
// or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once
#include <common/book_l2_map.hpp>

namespace ssc2ce {

class CexParser;

class CexBookL2 : public BookL2Map {
public:
  CexBookL2(const std::string &instrument) : BookL2Map{instrument} {}
  int64_t get_last_id() const { return last_id_; }
  void set_last_id(int64_t id) { last_id_ = id; }

  int64_t get_timestamp() const { return timestamp_; }
  void set_timestamp(int64_t timestamp) { timestamp_ = timestamp; }

protected:
  int64_t last_id_ = 0;
  int64_t timestamp_{};
  friend class CexParser;
};

} // namespace ssc2ce
