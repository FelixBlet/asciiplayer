cmake_minimum_required(VERSION 3.0)

project(asciiplayer)

add_executable(main main.cpp)

install(TARGETS main RUNTIME DESTINATION bin)

include_directories(/usr/include/libavformat/)

