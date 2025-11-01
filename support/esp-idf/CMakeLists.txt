cmake_minimum_required(VERSION 3.28)

include($ENV{IDF_PATH}/tools/cmake/idf.cmake)

idf_build_process(emdevif_esp_idf_support
    COMPONENTS esp32 freertos
    SDKCONFIG ${CMAKE_SOURCE_DIR}/sdkconfig
    BUILD_DIR ${CMAKE_BINARY_DIR}
)
