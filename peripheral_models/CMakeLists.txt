cmake_minimum_required(VERSION 3.28)

project(emdevif_peripheral_models CXX)

add_library(${PROJECT_NAME} STATIC)

target_link_libraries(${PROJECT_NAME}
    PUBLIC emdevif_core emdevif_util emdevif_peripheral_handle_map
    PRIVATE emdevif_compile_options
)

target_sources(${PROJECT_NAME}
    PUBLIC FILE_SET emdevif_peripheral_models_modules TYPE CXX_MODULES FILES
        Gpio.cppm
        Timer.cppm
        Pwm.cppm
        Serial.cppm
        Can.cppm
        Spi.cppm

    PUBLIC FILE_SET emdevif_peripheral_models_prv_modules TYPE CXX_MODULES FILES
        peripheral_models_error_handler.cppm
)
