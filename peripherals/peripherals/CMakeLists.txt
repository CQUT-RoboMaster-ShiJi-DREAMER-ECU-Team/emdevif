cmake_minimum_required(VERSION 3.28)

project(emdevif_peripheral_behaviours CXX)

add_library(${PROJECT_NAME} STATIC)

target_link_libraries(${PROJECT_NAME}
    PUBLIC emdevif_core emdevif_util emdevif_peripheral_handle_map emdevif_peripheral_models
    PRIVATE emdevif_compile_options
)

target_sources(${PROJECT_NAME}
    PUBLIC FILE_SET ${PROJECT_NAME}_modules TYPE CXX_MODULES FILES
        Gpio.cppm
        Pwm.cppm
        Serial.cppm
        Spi.cppm
        Timer.cppm
        Can.cppm

        peripheral_models_error_handler.cppm
)
