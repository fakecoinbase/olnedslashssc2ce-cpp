cmake_minimum_required(VERSION 3.15.0)
SET(PROJECT_VER "0.0.1")
project(ssc2ce-cpp VERSION ${PROJECT_VER} DESCRIPTION "set of C++ classes for ssc3ce")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(CTest)
enable_testing()

SET(SYSTEM PYBIND11_PYTHON_VERSION 3.8)
include_directories(SYSTEM ${CMAKE_SOURCE_DIR}/libs/include)
include_directories(SYSTEM ${CMAKE_SOURCE_DIR}/src)


add_subdirectory(fmt)
find_package(fmt)
include_directories(SYSTEM ${CMAKE_SOURCE_DIR}/fmt/include)

add_subdirectory(src)
add_subdirectory(pybind11)
add_subdirectory(python-bindings)
add_subdirectory(tests)

# set(CPACK_PROJECT_NAME ${PROJECT_NAME})
# set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
# include(CPack)