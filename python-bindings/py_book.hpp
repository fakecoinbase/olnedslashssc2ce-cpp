// Copyright Oleg Nedbaylo 2020.
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE
// or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once
#include <common/book_l2.hpp>
#include <pybind11/pybind11.h>

template <class BookBase = ssc2ce::BookL2>
class PyBook : public BookBase {
public:
  using BookBase::BookBase; // Inherit constructors
  double top_bid_price() { PYBIND11_OVERLOAD_PURE(double, BookBase, top_bid_price, ); }
  double top_ask_price() { PYBIND11_OVERLOAD_PURE(double, BookBase, top_ask_price, ); }
};
