// Copyright Oleg Nedbaylo 2020.
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE
// or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once
#include <common/parser.hpp>
// #include <functional>
// #include <map>
// #include <string>

namespace ssc2ce {

class DeribitParser : public Parser {
public:
  DeribitParser();
  ~DeribitParser() {}

  bool parse(const char *message) override;

private:
//   using ParseChannel = std::function<bool(const char *)>;
//   std::map<std::string, ParseChannel> channels_;
};

} // namespace ssc2ce