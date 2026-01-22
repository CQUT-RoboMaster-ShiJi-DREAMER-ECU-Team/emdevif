cmake_minimum_required(VERSION 3.28...4.0.2)

project(emdevif_peripheral_model CXX)

add_library(${PROJECT_NAME} STATIC)

target_link_libraries(${PROJECT_NAME}
    PUBLIC emdevif_core emdevif_util
    PRIVATE emdevif_compile_options
)

target_sources(${PROJECT_NAME}
    PUBLIC FILE_SET emdevif_peripheral_models_modules TYPE CXX_MODULES FILES
        GpioModel.cppm
        CanModel.cppm
        PwmModel.cppm
        SerialModel.cppm
        SpiModel.cppm
        TimerModel.cppm
)
