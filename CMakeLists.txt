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
add_executable(game_test_mavigierlafo game_test_mavigierlafo.c)

target_link_libraries(game_text game)

add_test(test_toccelli_dummy ./game_test_toccelli dummy)


add_test(test_masalin_dummy ./game_test_masalin dummy)
add_test(test_masalin_new_empty ./game_test_masalin new_empty)
add_test(test_masalin_get_color ./game_test_masalin get_color)


add_test(test_mavigierlafo_dummy ./game_test_mavigierlafo dummy)
