cmake_minimum_required(VERSION 3.28)

project(emdevif_esp_idf_support C CXX)

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME} INTERFACE
    $<TARGET_PROPERTY:idf::freertos,INTERFACE_INCLUDE_DIRECTORIES>
)
target_compile_definitions(${PROJECT_NAME} INTERFACE
    EMDEVIF_THREAD_USE_ESPIDF_FREERTOS=1
)

message("[${PROJECT_NAME}]: usage ESP-IDF.")
