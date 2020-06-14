include_directories(SYSTEM ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(
    cpp_tests
    main_test.cpp
    deribit_parser_test.cpp
)
               
target_link_libraries(
    cpp_tests
    ssc2ce)

add_test(NAME cpp_my_lib_test COMMAND cpp_tests)
