cmake_minimum_required(VERSION 3.15)

project(feigenbaums-rabbits)

#find_library(SDL2_LIBRARY NAMES SDL2)
find_package(SDL2)

add_executable(fs-bunnies feigenbaum.cc)
target_include_directories(fs-bunnies PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(fs-bunnies PRIVATE ${SDL2_LIBRARIES})
