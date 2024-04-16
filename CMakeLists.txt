cmake_minimum_required(VERSION 3.24)

project(ptc_mqtt LANGUAGES C CXX)

add_library(ptc src/main.c)
target_include_directories(ptc PRIVATE inc/)