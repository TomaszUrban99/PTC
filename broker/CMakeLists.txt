cmake_minimum_required(VERSION 3.24)

project(ptc_mqtt LANGUAGES C CXX)

add_library(tcp_connection src/tcp_connection.c)
target_include_directories(tcp_connection PRIVATE inc)

add_library(mqtt_utils src/mqtt_broker_utils.c)
target_include_directories(mqtt_utils PRIVATE inc)

add_library(mqtt_broker src/mqtt_broker.c)
target_include_directories(mqtt_broker PRIVATE inc)
target_link_libraries(mqtt_broker mqtt_utils)

add_executable(ptc_server src/main.c)
target_include_directories(ptc_server PUBLIC inc)
target_link_libraries(ptc_server mqtt_broker tcp_connection)