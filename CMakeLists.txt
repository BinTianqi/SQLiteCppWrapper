project(sqlite_cpp_wrapper LANGUAGES CXX)

if(NOT TARGET SQLite::SQLite3)
    message(FATAL_ERROR "SQLite::SQLite3 target not found. Please provide it.")
endif()

file(GLOB_RECURSE SRCS ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)
add_library(sqlite_cpp_wrapper ${SRCS})
target_link_libraries(sqlite_cpp_wrapper PUBLIC SQLite::SQLite3)
target_include_directories(sqlite_cpp_wrapper PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
