# 1. ${PROJECT_SOURCE_DIR}：
# 定义：这是 CMake 内置的一个变量，表示 项目的根目录（即 CMakeLists.txt 文件所在的目录）。这个路径通常是你执行 cmake 命令时指定的源代码目录。
# 用途：它常用于指定文件的相对路径，或者作为查找源代码、包含文件等的基准路径。
# 例子：file(GLOB_RECURSE hh_sources ${PROJECT_SOURCE_DIR}/src/*.cc) 会在项目根目录的 src/ 目录中查找 .cc 文件。
# 2. ${LIB_NAME}：
# 定义：这是一个用户自定义的变量，通常作为宏或函数的参数传递。其值是你定义的库的名称（如 tinyxml2）。
# 用途：在你的 add_thirdparty_lib 宏中，LIB_NAME 代表第三方库的名称，并通过 ${LIB_NAME} 引用它。例如，LIB_NAME 可以是 tinyxml2，那么 ${LIB_NAME} 就会被解析成 tinyxml2 字符串。
# 例子：如果在调用 add_thirdparty_lib(tinyxml2) 时，LIB_NAME 的值是 tinyxml2，那么 ${LIB_NAME} 就会被替换为 tinyxml2。
#  ${${LIB_NAME}_sources}：
# 定义：这是一个 嵌套的变量引用，它的作用是动态地引用另一个变量的值。具体来说，${LIB_NAME}_sources 是一个变量名，其名称是由 ${LIB_NAME} 的值动态决定的。
# 用途：在宏 add_thirdparty_lib 中，${LIB_NAME}_sources 代表该库的源文件列表（例如，tinyxml2_sources）。通过 ${${LIB_NAME}_sources} 的方式，CMake 会首先解析 ${LIB_NAME}，然后再解析出最终的变量名，并获取其值（即该库的源文件列表）。
# 例子：假设 LIB_NAME 为 tinyxml2，那么 ${${LIB_NAME}_sources} 实际上会被解析为 ${tinyxml2_sources}，从而获取 tinyxml2_sources 变量的值（即 tinyxml2 库的源代码文件列表）。



# 查找所有测试源文件
file(GLOB hh_TEST_SOURCES "${PROJECT_SOURCE_DIR}/test/*/*test.cc")

######################################################################################################################
# DEPENDENCIES
######################################################################################################################

message(STATUS "hh_TEST_SOURCES: ${hh_TEST_SOURCES}")

find_library(REDIS_PLUS_PLUS_LIB redis++ PATHS ${REDIS_PLUS_PLUS_LIB_PATH})
if (NOT REDIS_PLUS_PLUS_LIB)
    message(FATAL_ERROR "Could not find redis++ library!")
endif()
message(STATUS "Redis++ path: ${REDIS_PLUS_PLUS_LIB}")

# 查找 valgrind 工具，如果找不到，输出警告信息
find_program(VALGRIND_BIN valgrind)
if ("${VALGRIND_BIN}" STREQUAL "VALGRIND_BIN-NOTFOUND")
    message(WARNING "hhSearchEngine/test couldn't find valgrind.")
else()
    message(STATUS "hhSearchEngine/test found valgrind at ${VALGRIND_BIN}")
endif()

# 1. 查找 Boost 库，指定 locale 组件
find_package(Boost REQUIRED COMPONENTS locale)

# 设置 valgrind 选项，用于内存泄漏检查等
set(VALGRIND_OPTIONS
        --error-exitcode=1                      # 如果检测到泄漏，返回非零值
        # --gen-suppressions=all  # 注释掉这一行以启用泄漏抑制语法
        --leak-check=full                       # 进行详细的泄漏信息报告
        --soname-synonyms=somalloc=*jemalloc*   # 也拦截 jemalloc 分配器
        --trace-children=yes                    # 跟踪子进程
        --track-origins=yes                     # 跟踪未初始化值的来源
        )
# 指定 valgrind 抑制文件的位置
set(VALGRIND_SUPPRESSIONS_FILE "${PROJECT_SOURCE_DIR}/build_support/valgrind.supp")

######################################################################################################################
# MAKE TARGETS
######################################################################################################################

##########################################
# 自定义目标 "make check-tests"
##########################################
# 添加一个自定义目标 'build-tests'，该目标仅显示所有测试，而不实际运行它们
add_custom_target(build-tests COMMAND ${CMAKE_CTEST_COMMAND} --show-only)
# 添加一个自定义目标 'check-tests'，该目标执行所有的单元测试
add_custom_target(check-tests COMMAND ${CMAKE_CTEST_COMMAND} --verbose)

##########################################
# 为每个测试源文件创建独立的目标
##########################################
foreach (hh_test_source ${hh_TEST_SOURCES})
    # 从源文件中提取文件名（不包括路径）
    get_filename_component(hh_test_filename ${hh_test_source} NAME)
    # 去掉文件扩展名，生成测试的名称
    string(REPLACE ".cc" "" hh_test_name ${hh_test_filename})

    # 创建独立的测试执行目标，并将其排除在默认构建目标之外
    add_executable(${hh_test_name} EXCLUDE_FROM_ALL ${hh_test_source})
    # 将当前测试目标添加到 'build-tests' 和 'check-tests' 的依赖中，确保它们在构建时被运行
    add_dependencies(build-tests ${hh_test_name})
    add_dependencies(check-tests ${hh_test_name})
    
    # 设置编译选项，这里使用 C++17 标准
    target_compile_options(${hh_test_name} PRIVATE -std=c++17 )
    # 也可以启用一些额外的编译选项，比如 'UNIT_TEST' 宏来控制单元测试的编译
    # target_compile_options(${hh_test_name} PRIVATE -std=c++17 -DUNIT_TEST)

    # 将共享库和其他依赖链接到测试目标上
    target_link_libraries(${hh_test_name} hh_shared re2 spdlog::spdlog Boost::locale redis++ )

    # 设置测试目标的输出目录及运行时命令
    set_target_properties(${hh_test_name}
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/test"  # 将生成的可执行文件放入测试目录
        COMMAND ${hh_test_name}  # 设置运行时执行的命令
    )
    message(STATUS "CMAKE_BINARY_DIR: ${CMAKE_BINARY_DIR}")
    
    # 进一步指定编译选项和链接选项
    # find_library(REDIS_PLUS_PLUS_LIB redis++)
    target_compile_options(${hh_test_name} PRIVATE -std=c++17)
    target_link_options(${hh_test_name} PRIVATE -lredis++)
    target_link_options(${hh_test_name} PRIVATE ${REDIS_PLUS_PLUS_LIB})

    # 使用 CTest 将测试添加到构建系统中，测试将与 valgrind 一起运行，检查内存泄漏
    add_test(${hh_test_name} ${VALGRIND_BIN} ${VALGRIND_OPTIONS}
    --suppressions=${VALGRIND_SUPPRESSIONS_FILE} ${CMAKE_BINARY_DIR}/test/${hh_test_name}
    --gtest_color=yes --gtest_output=xml:${CMAKE_BINARY_DIR}/test/unit_${test_name}.xml)
    
    # 另外，也可以直接运行测试而不使用 valgrind
    add_test(${hh_test_name} ${CMAKE_BINARY_DIR}/test/${hh_test_name} --gtest_color=yes
            --gtest_output=xml:${CMAKE_BINARY_DIR}/test/${hh_test_name}.xml)
endforeach(hh_test_source ${hh_TEST_SOURCES})
