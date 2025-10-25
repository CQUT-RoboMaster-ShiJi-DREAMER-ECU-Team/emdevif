# emdevif

emdevif: embedded developing interface 嵌入式开发接口。

## todo

- [] 合并 [peripheral_handle_map](peripheral_handle_map) 与 [peripheral](peripheral)
- [] 把 logger、system 模块拆分成单独的仓库
- [] 把环形缓冲区从 util 移动到 data_container

## 子模块

* [core](./core) 核心部分，所有库都需要链接它。
* [logger](./logger) 日志。
* [timeline](./timeline) 微秒级时间戳。
* [peripherals](./peripheral) 对外设的操作接口。
* [system](./system) 对操作系统（例如 FreeRTOS）调用的封装。
* [util](./util) 其他实用功能。

## 相关文档

[命名约定](.)  (todo)

## 使用方法

由于此项目采用了比较分散的模块化设计，您可以根据需要选择性地使用其中的某些模块，而未使用的模块完全不会参与编译。
但这样也导致了使用相对比较复杂。下面将介绍如何在您的工程中使用 emdevif。

### 基本配置结构

假如您的工程目录如下：

```
project_root
├── inc/
│   └── ...
├── src/
│   └── ...
├── CMakeLists.txt
└── ...
```

您可以使用 `git clone` 或 `git submodule add` 将本仓库添加到一个合适的目录中，例如：

```Shell
git submodule add https://github.com/CQUT-RoboMaster-ShiJi-DREAMER-ECU-Team/emdevif.git emdevif
```

这样，文件结构将会变成：

```
project_root
├── inc/
│   └── ...
├── src/
│   └── ...
├── CMakeLists.txt
├── emdevif/
│   └── ...
└── ...
```

假如您的 CMakeLists.txt 内容如下：

```CMake
cmake_minimum_required(VERSION 3.28)
project(your_project_name C CXX)

add_executable(${PROJECT_NAME}
    src/main.c
    # 其他源文件...
)
target_include_directories(${PROJECT_NAME} PRIVATE
    inc
    # 其他头文件路径...
)
```

我们将以这个结构为例，逐步介绍如何使用 emdevif。

首先，参考子模块列表，决定您需要哪些功能。除了 core 模块，其他模块均为可选模块。
假如您需要使用 logger 和 util 模块，则需要在 CMakeLists.txt 中设置相应的选项：

```CMake
cmake_minimum_required(VERSION 3.28)
project(your_project_name C CXX)

set(CMAKE_C_STANDARD 11)            # 最低限制是 C11
set(CMAKE_CXX_STANDARD 20)          # 最低限制是 C++20
set(CMAKE_CXX_SCAN_FOR_MODULES ON)  # 启用 C++ 模块支持（必须设置为 ON）

add_executable(${PROJECT_NAME}
    src/main.c
    # 其他源文件...
)
target_include_directories(${PROJECT_NAME} PRIVATE
    inc
    # 其他头文件路径...
)

# 您需要设置这个变量来启用所需的模块
set(EMDEVIF_ENABLED_MODULES
    "logger;util"  # 使用分号分隔模块名称（不需要手动增加 core 模块，因为它必定会被使用）
    CACHE STRING "" FORCE
)  # 您也可以通过命令行传递这个变量，例如: cmake -DEMDEVIF_ENABLED_MODULES="logger;util" ...

add_subdirectory(emdevif)  # 请注意，这一条语句需要放在设置变量之后

target_link_libraries(${PROJECT_NAME} PRIVATE emdevif)  # 链接 emdevif 库
```

这样基本上就大功告成了！不过有些子模块还包含一些其他的配置选项，您可以参考各个子模块的 README.md 来进行配置。

我们建议您单独创建一个 `emdevif_config.cmake` 文件来管理 emdevif 的配置选项，这样可以使得您的
CMakeLists.txt 更加清晰，并且便于管理子模块的配置。例如：

```
project_root
├── cmake/  # 存放 CMake 配置文件的目录
│   ├── emdevif_config.cmake
│   └── ...
├── inc/
│   └── ...
├── src/
│   └── ...
├── CMakeLists.txt
├── emdevif/
│   └── ...
└── ...
```

然后修改您的 CMakeLists.txt 如下：

```CMake
# project_root/CMakeLists.txt

cmake_minimum_required(VERSION 3.28)
project(your_project_name C CXX)

set(CMAKE_C_STANDARD 11)            # 最低限制是 C11
set(CMAKE_CXX_STANDARD 20)          # 最低限制是 C++20
set(CMAKE_CXX_SCAN_FOR_MODULES ON)  # 启用 C++ 模块支持（必须设置为 ON）

add_executable(${PROJECT_NAME}
    src/main.c
    # 其他源文件...
)
target_include_directories(${PROJECT_NAME} PRIVATE
    inc
    # 其他头文件路径...
)

include(cmake/emdevif_config.cmake)  # 包含配置文件
target_link_libraries(${PROJECT_NAME} PRIVATE emdevif)
```

```CMake
# cmake/emdevif_config.cmake

# 按照相同的方式设置所需的模块
set(EMDEVIF_ENABLED_MODULES "logger;util" CACHE STRING "" FORCE)
add_subdirectory(emdevif)

# 尽管这样看上去有些多余，但如果子模块的配置选项较多，这样做会使得 CMakeLists.txt 更加清晰。
```

在子模块的 README.md 中，将会提供 cmake/emdevif_config.cmake 的示例，您可以将其内容复制到您的配置文件中并加以修改即可。

### 由用户自定义的实现模块 emdevif_user_declares

有一部分子模块（例如 logger、timeline）需要用户提供一些底层实现，例如如何输出日志、如何获取时间戳等。这些实现被称为“用户自定义的实现模块”（user-declared
implementation modules），简称 emdevif_user_declares。

您需要在您的工程中创建这个模块，并在 CMakeLists.txt 中将其添加为一个目标：

```
project_root
├── cmake/  # 存放 CMake 配置文件的目录
│   ├── emdevif_config.cmake
│   └── ...
├── inc/
│   └── ...
├── src/
│   └── ...
├── CMakeLists.txt
├── emdevif/
│   └── ...
├── emdevif_user_declares/  # 用户自定义的实现模块
│   ├── emdevif_user_declares.cppm
│   └── ...
└── ...
```

```CMake
# project_root/CMakeLists.txt

# 按照相同的方式配置
cmake_minimum_required(VERSION 3.28)
project(your_project_name C CXX)

set(CMAKE_C_STANDARD 11)            # 最低限制是 C11
set(CMAKE_CXX_STANDARD 20)          # 最低限制是 C++20
set(CMAKE_CXX_SCAN_FOR_MODULES ON)  # 启用 C++ 模块支持（必须设置为 ON）

add_executable(${PROJECT_NAME} src/main.c)
target_include_directories(${PROJECT_NAME} PRIVATE inc)

include(cmake/emdevif_config.cmake)
target_link_libraries(${PROJECT_NAME} PRIVATE emdevif)

# 添加用户自定义的实现模块
add_library(emdevif_user_declares STATIC)
target_sources(emdevif_user_declares
    PUBLIC FILE_SET emdevif_user_declares_module TYPE CXX_MODULES
    FILES emdevif_user_declares/emdevif_user_declares.cppm  # 您的实现文件，需要提供 C++ 模块
)
```

```C++
// emdevif_user_declares/emdevif_user_declares.cppm

module;

// 可以在这里添加头文件

export module emdevif.userDeclares;  // 导出的模块名称必须与它相同

// 可以在这里导入其他模块

export namespace emdevif::user_declares  // 命名空间也必须与它相同
{

// 在这里实现所需的功能，例如 logger 模块需要实现的日志输出函数。
// 具体请参考各个子模块的 README.md

// 当然，您也可以将实现分散在多个文件，或者放在不同的模块分区内，只要确保它们都被包含在
// emdevif.userDeclares 模块中并且会被导出即可。

}
```
