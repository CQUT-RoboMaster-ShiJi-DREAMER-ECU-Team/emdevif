# emdevif

emdevif: embedded developing interface 嵌入式开发接口。

## 子模块

* [core](./core) 核心部分，所有库都需要链接它。
* [logger](./logger) 日志。
* [timeline](./timeline) 微秒级时间戳。
* [peripherals](./peripheral) 对外设的操作接口。
* [system](./system) 对操作系统（例如 FreeRTOS）调用的封装。
* [util](./util) 其他实用功能。

## 外部模块

* [emdevif_stm32_peripheral](https://github.com/CQUT-RoboMaster-ShiJi-DREAMER-ECU-Team/emdevif_stm32_peripheral.git)
  提供一部分 STM32 HAL/LL 外设库的封装
* [emdevif_test_framework](https://github.com/CQUT-RoboMaster-ShiJi-DREAMER-ECU-Team/emdevif_test_framework.git) 单元测试框架

## 相关文档

- [编码规范](https://github.com/CQUT-RoboMaster-ShiJi-DREAMER-ECU-Team/rmdev_developing_or_testing_environment/blob/main/docs/emdevif-coding-style.md)
- [rmdev](https://github.com/CQUT-RoboMaster-ShiJi-DREAMER-ECU-Team/rmdev.git)

## 模块与头文件说明

rmdev、emdevif 库提供了两种导入方式：模块导入，和头文件导入。这两种方式根据您的条件来选择使用。模块导入需要您的编译器支持
C++20
模块，并且需要在 CMake 中启用模块支持（`set(CMAKE_CXX_SCAN_FOR_MODULES ON CACHE INTERNAL "" FORCE)`
），它可以提供更好的编译性能和更清晰的接口；头文件导入则兼容性、LSP 更友好，但可能会导致较长的编译时间，并且会暴露部分实现细节。

条件允许的情况下，我们建议您使用模块导入的方式，这样可以获得更好的开发体验和编译性能。

经测试，目前支持模块导入的工具链包括：

* CMake >= 3.28
* arm-none-eabi-g++ >= 14.2

经测试，目前对模块支持较好的开发环境只有启用了 Nova 引擎的 CLion 2024.2 以及之后的版本，其他 IDE（例如 Visual
Studio、VSCode）可能存在一些问题，例如无法正确识别模块接口、无法正确解析模块间的依赖关系等。

**在启用模块的情况下，请勿同时引入头文件**（除了一些特殊情况，会有相应的说明），否则可能导致编译失败。

模块的命名与头文件的命名方式完全一致，例如：

```C++
import emdevif.core.error_handler;
// 等价于
#include "emdevif/core/error_handler.hpp"
```

在文档中，我们统一以模块导入的方式来介绍接口的使用方法，但您完全可以将模块导入替换为头文件导入，接口的使用方法完全相同。

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
git submodule add https://github.com/CQUT-RoboMaster-ShiJi-DREAMER-ECU-Team/emdevif.git emdevif_collection/emdevif
```

这样，文件结构将会变成：

```
project_root
├── inc/
│   └── ...
├── src/
│   └── ...
├── CMakeLists.txt
├── emdevif_collection/
│   └── emdevif/
│       └── ...
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
set(CMAKE_CXX_SCAN_FOR_MODULES ON)  # 启用 C++ 模块支持（如果需要使用模块，则必须启用）

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
    CACHE INTERNAL "" FORCE
)  # 您也可以通过命令行传递这个变量，例如: cmake -DEMDEVIF_ENABLED_MODULES="logger;util" ...

add_subdirectory(emdevif_collection/emdevif)  # 请注意，这一条语句需要放在设置变量之后

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
├── emdevif_collection/
│   └── emdevif/
│       └── ...
└── ...
```

然后修改您的 CMakeLists.txt 如下：

```CMake
# project_root/CMakeLists.txt

cmake_minimum_required(VERSION 3.28)
project(your_project_name C CXX)

set(CMAKE_C_STANDARD 11)            # 最低限制是 C11
set(CMAKE_CXX_STANDARD 20)          # 最低限制是 C++20
set(CMAKE_CXX_SCAN_FOR_MODULES ON)  # 启用 C++ 模块支持（如果需要使用模块，则必须启用）

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
set(EMDEVIF_ENABLED_MODULES "logger;util" CACHE INTERNAL "" FORCE)
add_subdirectory(${CMAKE_SOURCE_DIR}/emdevif_collection/emdevif)

# 尽管这样看上去有些多余，但如果子模块的配置选项较多，这样做会使得 CMakeLists.txt 更加清晰。
```

在子模块的 README.md 中，将会提供 cmake/emdevif_config.cmake 的示例，您可以将其内容复制到您的配置文件中并加以修改即可。

### 由用户自定义的实现模块 emdevif_user_declares

有一部分子模块（例如 logger、timeline）需要用户提供一些底层实现，例如如何输出日志、如何获取时间戳等。这些实现被称为“用户自定义的实现模块”（user-declared
implementation modules），简称 emdevif_user_declares。

#### 使用模块的情况

如果您使用了模块导入的方式，那么您需要为 emdevif_user_declares 模块提供一个实现模块，并且将其添加到 CMake
中。这个模块需要导出一个特定的命名空间 `emdevif::user_declares`，并且在其中导出实现所需的功能。

首先需要在您的工程中创建这个模块，并在 CMakeLists.txt 中将其添加为一个目标：

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
├── emdevif_collection/
│   ├── emdevif/
│   │   └── ...
│   └── emdevif_user_declares/  # 用户自定义的实现模块
│       ├── emdevif_user_declares.cppm
│       └── ...
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
    FILES emdevif_collection/emdevif_user_declares/emdevif_user_declares.cppm  # 您的实现文件，需要提供为 C++ 模块
)
target_compile_definitions(emdevif_user_declares PUBLIC
    EMDEVIF_USER_DECLARES_PROVIDE_MODULE=1  # 使用模块时，还需要提供这个宏定义，以便 emdevif 的模块能够正确地导入用户自定义的实现模块
)
target_link_libraries(emdevif_user_declares
    # 可以链接其他库
    # 注意，部分库（相应库的文档会有说明）不能在这里链接，否则会产生循环依赖。
)
```

```C++
// emdevif_collection/emdevif_user_declares/emdevif_user_declares.cppm

module;

// 可以在这里添加头文件

export module emdevif.user_declares;  // 导出的模块名称必须与它相同

// 可以在这里导入其他模块
// 注意，部分 emdevif 的模块（相应模块内会有说明）不能在这里导入，否则会产生循环依赖。

export namespace emdevif::user_declares  // 命名空间也必须与它相同
{

// 在这里实现所需的功能，例如 logger 模块需要实现的日志输出函数。
// 具体请参考各个子模块的 README.md

// 当然，您也可以将实现分散在多个文件，或者放在不同的模块分区内，只要确保它们都被包含在
// emdevif.user_declares 模块中并且会被导出即可。

}
```

#### 使用头文件的情况

使用头文件时，用户自定义的实现模块则需要提供一个头文件，并且在其中定义一个特定的命名空间 `emdevif::user_declares`，然后在其中声明实现所需的功能。

首先需要在您的工程中创建这个模块，并在 CMakeLists.txt 中将其添加为一个目标：

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
├── emdevif_collection/
│   ├── emdevif/
│   │   └── ...
│   └── emdevif_user_declares/  # 用户自定义的实现模块
│       ├── inc/emdevif/user_declares.hpp  # 需要提供这个头文件，并且在其中声明实现所需的功能
│       └── ...
└── ...
```

```CMake
# project_root/CMakeLists.txt

# 按照相同的方式配置
cmake_minimum_required(VERSION 3.28)
project(your_project_name C CXX)

set(CMAKE_C_STANDARD 11)             # 最低限制是 C11
set(CMAKE_CXX_STANDARD 20)           # 最低限制是 C++20
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)  # 不使用模块，就不需要启用模块支持了

add_executable(${PROJECT_NAME} src/main.c)
target_include_directories(${PROJECT_NAME} PRIVATE inc)

include(cmake/emdevif_config.cmake)
target_link_libraries(${PROJECT_NAME} PRIVATE emdevif)

# 添加用户自定义的实现模块
# 根据您的需求决定库的类型，如果时纯头文件库，则使用 INTERFACE 类型；如果实现包含源文件，则使用 STATIC 或 SHARED 类型。
# 这里以纯头文件库为例：
add_library(emdevif_user_declares INTERFACE)
target_include_directories(emdevif_user_declares INTERFACE
    emdevif_collection/emdevif_user_declares/inc  # 包含用户自定义的实现模块的头文件路径
)
# target_compile_definitions(emdevif_user_declares PUBLIC
#     EMDEVIF_USER_DECLARES_PROVIDE_MODULE=1
# )  # 使用头文件时，**不能**定义这个宏，否则 emdevif 的模块会错误地认为用户提供了一个模块实现，从而在导入时产生错误。
target_link_libraries(emdevif_user_declares
    # 可以链接其他库
    # 注意，部分库（相应库的文档会有说明）不能在这里链接，否则会产生循环依赖。
)
```

```C++
// emdevif_collection/emdevif_user_declares/inc/emdevif/user_declares.hpp

#pragma once
#ifndef EMDEVIF_USER_DECLARES_HPP
#define EMDEVIF_USER_DECLARES_HPP

// 可以在这里添加头文件
// 注意，部分 emdevif 的头文件（相应模块内会有说明）不能在这里导入，否则会产生循环依赖。

namespace emdevif::user_declares  // 命名空间也必须与它相同
{

// 在这里实现所需的功能，例如 logger 模块需要实现的日志输出函数。
// 具体请参考各个子模块的 README.md

// 当然，您也可以将实现分散在多个文件，或者放在不同的模块分区内，只要确保它们都被包含在
// emdevif/user_declares.hpp 中即可。

}

#endif  // !EMDEVIF_USER_DECLARES_HPP
```

### CMake 变量配置

| CMake 变量                | 类型     | 默认值                              | 说明                            |
|-------------------------|--------|----------------------------------|-------------------------------|
| EMDEVIF_USE_CPP_MODULES | Bool   | ON                               | 是否使用 C++ 模块支持（如果需要使用模块，则必须启用） |
| EMDEVIF_ENABLE_TESTS    | Bool   | OFF                              | 是否启用测试                        |
| EMDEVIF_USE_STM32CUBEMX | Bool   | OFF                              | 是否使用 STM32CubeMX              |
| EMDEVIF_ENABLED_MODULES | String | `${emdevifAllModules}`(默认开启所有模块) | 要使用的模块，模块名之间用分号间隔             |

## 测试

见 [test](./test)。
