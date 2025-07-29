"""
emdevif_stm32cubemx_processor 是一个用于将 STM32CubeMX 生成的 CMakeLists.txt 进行处理以适配 emdevif
的一个脚本

Requires:
    Python3

Example:
    python emdevif_stm32cubemx_processor cmake/stm32cubemx/CMakeLists.txt
"""

import sys
import os


def get_caller_path():
    """
    获得调用这个程序的绝对路径
    :return: 调用这个程序的绝对路径
    """
    return os.path.abspath(os.getcwd())


if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Use: python emdevif_stm32cubemx_processor <process file>")
        exit(1)

    argv_file_name = sys.argv[1]
    argv_file_name.replace("\\", "/")

    process_file_name = get_caller_path()
    process_file_name.replace("\\", "/")
    process_file_name += "/" + argv_file_name

    process_file = open(process_file_name, "r")

    file_str = process_file.readlines()

    process_file.close()

    WILL_REPLACE_COMMENT_FLAG1: str = "# Link directories setup"
    WILL_REPLACE_COMMAND1: str = "target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE ${MX_LINK_DIRS})"
    WILL_REPLACE_COMMENT_FLAG2: str = "# Add libraries to the project"
    WILL_REPLACE_COMMAND2: str = "target_link_libraries(${CMAKE_PROJECT_NAME} ${MX_LINK_LIBS})"
    for i in range(0, len(file_str)):
        if file_str[i].find(WILL_REPLACE_COMMENT_FLAG1) != -1:
            i += 1
            if file_str[i].find(WILL_REPLACE_COMMAND1) != -1:
                if file_str[i][0] != "#":
                    result_str = "#" + file_str[i]
                    file_str[i] = result_str

        if file_str[i].find(WILL_REPLACE_COMMENT_FLAG2) != -1:
            i += 1
            if file_str[i].find(WILL_REPLACE_COMMAND2) != -1:
                if file_str[i][0] != "#":
                    result_str = "#" + file_str[i]
                    file_str[i] = result_str

    result_str = ""
    for i in range(0, len(file_str)):
        result_str += file_str[i]

    process_file = open(process_file_name, "w")
    process_file.write(result_str)
    process_file.close()

    exit(0)
