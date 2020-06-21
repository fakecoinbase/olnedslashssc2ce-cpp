#pragma once

#include <common/book_l2.hpp>
#include <common/parser.hpp>
#include <pybind11/pybind11.h>

template <class ParserBase = ssc2ce::Parser>
class PyParser : public ParserBase {
public:
  using ParserBase::ParserBase; // Inherit constructors
  bool parse(const char *message) { PYBIND11_OVERLOAD_PURE(bool, ParserBase, parse, message); }
  ssc2ce::BookL2 const *get_book() { PYBIND11_OVERLOAD_PURE(ssc2ce::BookL2 const *, ParserBase, get_book, ); }
};

template <class Implementation>
class PyParserImplementation : public PyParser<Implementation> {
public:
  using PyParser<Implementation>::PyParser; // Inherit constructors

  // Override pure virtual methods with non-pure ones:
  bool parse(const char *message) override { PYBIND11_OVERLOAD(bool, Implementation, parse, message); }
//   ssc2ce::BookL2 const *get_book() override { PYBIND11_OVERLOAD(ssc2ce::BookL2 const *, Implementation, get_book, ); }
};