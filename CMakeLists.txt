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
target_link_libraries(game_test_toccelli game)
target_link_libraries(game_test_masalin game)
target_link_libraries(game_test_mavigierlafo game)



add_test(test_mavigierlafo_dummy ./game_test_mavigierlafo dummy)
add_test(test_mavigierlafo_won ./game_test_mavigierlafo test_won)
add_test(test_mavigierlafo_nb_neighbors ./game_test_mavigierlafo test_nb_neighbors)
add_test(test_mavigierlafo_get_status ./game_test_mavigierlafo test_get_status)
add_test(test_mavigierlafo_default ./game_test_mavigierlafo test_default)
add_test(test_mavigierlafo_print ./game_test_mavigierlafo test_print)
