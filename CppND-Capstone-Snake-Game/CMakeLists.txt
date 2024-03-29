cmake_minimum_required(VERSION 3.11.3)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-std=c++17)

set(CXX_FLAGS "-Wall -pthread")
set(CMAKE_CXX_FLAGS, "${CXX_FLAGS}")
project(SDL2Test)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(SDL2 REQUIRED)
find_package(Threads REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS} src)

# Find all executables
file(GLOB project_SRCS src/*.cpp) #src/*.h

add_executable(SnakeGame ${project_SRCS})
# add_executable(SnakeGame src/main.cpp src/game.cpp src/GameObject.cpp src/map.cpp src/SwitchableColor.h src/Movables.h src/controller.cpp src/renderer.cpp src/snake.cpp src/player.cpp src/rival.cpp src/food.cpp src/bullet.cpp)
string(STRIP ${SDL2_LIBRARIES} SDL2_LIBRARIES)
target_link_libraries(SnakeGame ${SDL2_LIBRARIES})
target_link_libraries(SnakeGame ${CMAKE_THREAD_LIBS_INIT})
