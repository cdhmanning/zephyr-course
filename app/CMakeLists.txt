cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED)
project(my_zephyr_app)

target_include_directories(app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_sources(app PRIVATE src/main.cpp)

add_subdirectory(drivers/our_sensorled)
