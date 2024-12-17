# 1. ${PROJECT_SOURCE_DIR}：
# 定义：这是 CMake 内置的一个变量，表示 项目的根目录（即 CMakeLists.txt 文件所在的目录）。这个路径通常是你执行 cmake 命令时指定的源代码目录。
# 用途：它常用于指定文件的相对路径，或者作为查找源代码、包含文件等的基准路径。
# 例子：file(GLOB_RECURSE simple_sources ${PROJECT_SOURCE_DIR}/src/*.cc) 会在项目根目录的 src/ 目录中查找 .cc 文件。
# 2. ${LIB_NAME}：
# 定义：这是一个用户自定义的变量，通常作为宏或函数的参数传递。其值是你定义的库的名称（如 tinyxml2）。
# 用途：在你的 add_thirdparty_lib 宏中，LIB_NAME 代表第三方库的名称，并通过 ${LIB_NAME} 引用它。例如，LIB_NAME 可以是 tinyxml2，那么 ${LIB_NAME} 就会被解析成 tinyxml2 字符串。
# 例子：如果在调用 add_thirdparty_lib(tinyxml2) 时，LIB_NAME 的值是 tinyxml2，那么 ${LIB_NAME} 就会被替换为 tinyxml2。
#  ${${LIB_NAME}_sources}：
# 定义：这是一个 嵌套的变量引用，它的作用是动态地引用另一个变量的值。具体来说，${LIB_NAME}_sources 是一个变量名，其名称是由 ${LIB_NAME} 的值动态决定的。
# 用途：在宏 add_thirdparty_lib 中，${LIB_NAME}_sources 代表该库的源文件列表（例如，tinyxml2_sources）。通过 ${${LIB_NAME}_sources} 的方式，CMake 会首先解析 ${LIB_NAME}，然后再解析出最终的变量名，并获取其值（即该库的源文件列表）。
# 例子：假设 LIB_NAME 为 tinyxml2，那么 ${${LIB_NAME}_sources} 实际上会被解析为 ${tinyxml2_sources}，从而获取 tinyxml2_sources 变量的值（即 tinyxml2 库的源代码文件列表）。


# 指定最低 CMake 版本
cmake_minimum_required(VERSION 3.10)

# 设置项目名称、版本和使用的语言
project(hh_search_engine VERSION 0.1.0 LANGUAGES CXX)

# 启用编译命令导出（例如用于 clang-tidy）
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) 

# 启用共享库的构建
set(BUILD_SHARED_LIBS ON)

# 设置 C++17 编译标准
set(CMAKE_CXX_STANDARD 17)              # Compile as C++17
set(CMAKE_CXX_STANDARD_REQUIRED ON)     # 强制要求使用 C++17 标准

# 启用测试功能
include(CTest)
enable_testing()

# set(CMAKE_BINARY_DIR ${PROJECT_SOURCE_DIR}/build)


######################################################################################################################
# 编译器和链接器设置
######################################################################################################################

# 设置调试模式下的编译选项，禁用优化并启用调试信息
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -ggdb")

# 设置链接器选项，启用位置无关代码（PIC）以支持共享库
set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -fPIC")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fPIC")
set(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS} -fPIC")

# 为 GCC 设置额外的链接选项（生成位置无关代码）
set(GCC_COVERAGE_LINK_FLAGS    "-fPIC")

# 输出当前的编译选项，以便调试
message(STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
message(STATUS "CMAKE_CXX_FLAGS_DEBUG: ${CMAKE_CXX_FLAGS_DEBUG}")
message(STATUS "CMAKE_EXE_LINKER_FLAGS: ${CMAKE_EXE_LINKER_FLAGS}")
message(STATUS "CMAKE_SHARED_LINKER_FLAGS: ${CMAKE_SHARED_LINKER_FLAGS}")

# 设置 C++ 编译选项（可以添加更多警告选项以提高代码质量）
set(SRC_CXX_FLAGS 
                "-std=c++17" 
                # "-Wall"      # 开启所有警告
                # "-Wextra"    # 开启额外警告
                # "-Wpedantic" # 强制遵守 C++ 标准
                # "-Werror"    # 将警告视为错误
                # "-Wconversion"  # 防止不必要的类型转换
                # "-Wfloat-equal" # 防止浮点数比较相等
                # "-Wformat-nonliteral" # 检查格式化字符串
                # "-Wformat-security"  # 检查格式化字符串的安全性
                # "-Wmissing-declarations"  # 检查缺少声明
                # "-Wshadow"  # 防止变量名遮蔽
                )

######################################################################################################################
# 包含头文件目录
######################################################################################################################

# 设置源代码、测试代码和第三方库的头文件目录
set(SRC_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/src/include)
set(TEST_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/test/include)
set(THIRD_PARTY_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/third_party)

include_directories(${PROJECT_SOURCE_DIR}/third_party/doctest/)
# include_directories(${PROJECT_SOURCE_DIR}/src/include/)
include_directories(/home/hh/searchEngine/src/include/)

# 将头文件目录添加到编译器的搜索路径
include_directories(
                    ${SRC_INCLUDE_DIR} 
                    ${TEST_INCLUDE_DIR} 
                    ${THIRD_PARTY_INCLUDE_DIR}
                    )

# 设置第三方库的链接目录
link_directories(${THIRD_PARTY_INCLUDE_DIR})

######################################################################################################################
# 查找和链接第三方库
######################################################################################################################

# 引入第三方库（spdlog 和 Threads）
include(build_support/third_party.cmake)
find_package(spdlog REQUIRED)  # 查找并引入 spdlog 库
find_package(Threads REQUIRED) # 查找并引入线程库

######################################################################################################################
# 设置安装目录
######################################################################################################################

# 引入 GNUInstallDirs 模块来管理安装目录
include(GNUInstallDirs)

# 设置输出目录
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY
    ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})  # 设置静态库输出目录
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY
    ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})  # 设置共享库输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY
    ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})  # 设置可执行文件输出目录

######################################################################################################################
# 子目录和构建设置
######################################################################################################################

# 添加子目录，分别编译源代码和测试代码
add_subdirectory(src)
add_subdirectory(test)

message(STATUS "SRC_INCLUDE_DIR: ${SRC_INCLUDE_DIR}")
message(STATUS "TEST_INCLUDE_DIR: ${TEST_INCLUDE_DIR}")
message(STATUS "THIRD_PARTY_INCLUDE_DIR: ${THIRD_PARTY_INCLUDE_DIR}")


# 获取 src 目录下所有源文件
file(GLOB_RECURSE SRC_SOURCES ${PROJECT_SOURCE_DIR}/src/*.cc)

list(REMOVE_ITEM SRC_SOURCES
  ${PROJECT_SOURCE_DIR}/src/server/keyCommander.cc
  ${PROJECT_SOURCE_DIR}/src/server/searchEngine.cc
  ${PROJECT_SOURCE_DIR}/src/server/webEngine.cc
  ${PROJECT_SOURCE_DIR}/src/client/client.cc)

# 添加 searchEngine.cpp 作为可执行文件的源文件
add_executable(webEngine ${PROJECT_SOURCE_DIR}/src/server/webEngine.cc ${SRC_SOURCES})
add_executable(keyCommander ${PROJECT_SOURCE_DIR}/src/server/keyCommander.cc ${SRC_SOURCES})
add_executable(searchEngine ${PROJECT_SOURCE_DIR}/src/server/searchEngine.cc ${SRC_SOURCES})
add_executable(client ${PROJECT_SOURCE_DIR}/src/client/client.cc ${SRC_SOURCES})
# message(STATUS "PROJECT_SOURCE_DIR: ${PROJECT_SOURCE_DIR}")
# 链接 hh_shared 库以及第三方库
target_link_libraries(webEngine PRIVATE hh_shared spdlog::spdlog Threads::Threads)
target_link_libraries(keyCommander PRIVATE hh_shared spdlog::spdlog Threads::Threads)
target_link_libraries(searchEngine PRIVATE hh_shared spdlog::spdlog Threads::Threads)
target_link_libraries(client PRIVATE hh_shared spdlog::spdlog Threads::Threads)
# 设置输出目录
set_target_properties(webEngine PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
    # 设置输出目录
set_target_properties(keyCommander PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
  # 设置输出目录
set_target_properties(searchEngine PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set_target_properties(client PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

message(STATUS "SRC_SOURCES: ${SRC_SOURCES}")

######################################################################################################################
# Doxygen 文档生成
######################################################################################################################

# # 包含 Doxygen 配置文件
# include(build_support/use_doxygen_doc.cmake)

# # 配置 Doxygen 文档目标
# add_doxygen_doc(
#   BUILD_DIR
#     ${CMAKE_CURRENT_BINARY_DIR}/_build     # 构建目录
#   DOXY_FILE
#     ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in  # Doxygen 配置文件
#   TARGET_NAME
#     docs   # 生成文档的目标名称
#   COMMENT
#     "HTML documentation"  # 生成 HTML 格式的文档
#   )

message(STATUS "CMAKE_BINARY_DIR: ${CMAKE_BINARY_DIR}")
