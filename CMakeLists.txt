project(sqlite_cpp_wrapper LANGUAGES CXX)

file(GLOB_RECURSE SRCS ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)
add_library(sqlite_cpp_wrapper ${SRCS})
target_include_directories(sqlite_cpp_wrapper PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
set_property(TARGET sqlite_cpp_wrapper PROPERTY CXX_STANDARD 20)
