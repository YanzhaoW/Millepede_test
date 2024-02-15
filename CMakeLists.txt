project(Millipede)

cmake_minimum_required(VERSION 3.24)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(ROOT REQUIRED COMPONENTS Core MathCore)
find_package(fmt REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(range-v3 REQUIRED)
find_package(Boost REQUIRED)

add_subdirectory(Mille)
add_subdirectory(src)

add_executable(mille main.cpp)
target_link_libraries(mille PRIVATE builder Mille)
