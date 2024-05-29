cmake_minimum_required(VERSION 3.24)

project(ptc_mqtt LANGUAGES C CXX)

add_library(tcp_connection src/tcp_connection.c)
target_include_directories(tcp_connection PRIVATE inc)

add_library(mqttutils src/mqtt_utils.c)
target_include_directories(mqttutils PUBLIC inc)

add_library(mqtt_client src/mqtt_client.c)
target_include_directories(mqtt_client PRIVATE inc)
target_link_libraries(mqtt_client mqttutils)

add_executable(ptc src/main.c)
target_include_directories(ptc PUBLIC inc)
target_link_libraries(ptc mqtt_client tcp_connection)