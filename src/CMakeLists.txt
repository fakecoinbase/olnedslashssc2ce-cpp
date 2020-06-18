cmake_minimum_required(VERSION 3.15.30)

add_library (
    ssc2ce 
    ssc2ce.cpp 
    coinbase_pro/parser.cpp
    deribit/parser.cpp
)