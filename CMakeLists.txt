cmake_minimum_required(VERSION 3.5)
project(engine)

set(CMAKE_CXX_STANDARD 14)

include_directories(/usr/include)

link_directories(/usr/local/lib)

add_executable(engine main.cpp)

target_link_libraries(engine raylib) 
