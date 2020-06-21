pybind11_add_module( 
    ssc2ce_cpp 
    ssc2ce_cpp.cpp 
    ../src/cex/parser.cpp
    ../src/coinbase_pro/parser.cpp
    ../src/deribit/parser.cpp
)
