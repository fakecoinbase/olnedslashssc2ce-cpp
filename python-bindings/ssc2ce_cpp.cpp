// Copyright Oleg Nedbaylo 2020.
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE
// or copy at http://www.boost.org/LICENSE_1_0.txt

#include "py_book.hpp"
#include <deribit/parser.hpp>
#include <pybind11/functional.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

PYBIND11_MODULE(ssc2ce_cpp, m)
{
  m.doc() = R"pbdoc(
        ssc2se_cpp
        -----------------------
        .. currentmodule:: ssc2se-cpp
        .. autosummary::
           :toctree: _generate
           BookL2
           subtract
    )pbdoc";

  py::class_<ssc2ce::BookL2, PyBook<>> book(m, "BookL2");

  book.def("top_bid_price", &ssc2ce::BookL2::top_bid_price)
      .def("top_ask_price", &ssc2ce::BookL2::top_ask_price)
      .def("instrument", &ssc2ce::BookL2::instrument)
      .def("__repr__",
           [](const ssc2ce::BookL2 &a) {
             return "<ssc2ce.BookL2{" + a.instrument() +
                    ", top_bid:" + std::to_string(a.top_bid_price()) +
                    ", top_ask:" + std::to_string(a.top_ask_price()) + "}>";
           });

  py::class_<ssc2ce::DeribitParser>(m, "DeribitParser")
      .def(py::init<>())
      .def("get_book",
           &ssc2ce::DeribitParser::get_book,
           py::return_value_policy::reference_internal)
      .def("set_on_book_setup", &ssc2ce::DeribitParser::set_on_book_setup)
      .def("set_on_book_update", &ssc2ce::DeribitParser::set_on_book_update)
      .def("parse", &ssc2ce::DeribitParser::parse);

#ifdef VERSION_INFO
  m.attr("__version__") = VERSION_INFO;
#else
  m.attr("__version__") = "dev";
#endif
}
