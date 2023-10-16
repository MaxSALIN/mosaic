cmake_minimum_required(VERSION 2.6)
project(src C)
include(CTest)
enable_testing()

set(CMAKE_VERBOSE_MAKEFILE on)
set(CMAKE_C_FLAGS "-std=c99 -g -Wall")
link_directories(${CMAKE_SOURCE_DIR})

add_executable(game_text game_text.c)
add_executable(game_test_toccelli game_test_toccelli.c)
add_executable(game_test_masalin game_test_masalin.c)

target_link_libraries(game_text game)

add_test(game_test_toccelli_dummy ./game_test_toccelli dummy)
add_test(test_masalin_dummy ./game_test_masalin dummy)
