cmake_minimum_required(VERSION 3.15)

project(feigenbaums-rabbits)

find_package(SDL2 REQUIRED)
add_executable(fs-bunnies feigenbaum.cc)
target_link_libraries(fs-bunnies PRIVATE SDL2::SDL2)
