# This is hacky :(
# 通过 GLOB_RECURSE 搜索并获取 src 目录下的所有 .cc 或 .cpp 文件，并将它们添加到 hh_sources 变量中
# 这会递归地查找项目中的所有源代码文件
# file(GLOB_RECURSE hh_sources ${PROJECT_SOURCE_DIR}/src/*/*.cc ${PROJECT_SOURCE_DIR}/src/*/*/*.cc)
# list(REMOVE_ITEM hh_sources ${PROJECT_SOURCE_DIR}/src/server/*.cc ${PROJECT_SOURCE_DIR}/src/client/*.cc)
file(GLOB_RECURSE hh_sources ${PROJECT_SOURCE_DIR}/src/*/*.cc ${PROJECT_SOURCE_DIR}/src/*/*/*.cc)
list(REMOVE_ITEM hh_sources
  ${PROJECT_SOURCE_DIR}/src/server/keyCommander.cc
  ${PROJECT_SOURCE_DIR}/src/server/searchEngine.cc
  ${PROJECT_SOURCE_DIR}/src/server/webEngine.cc
  ${PROJECT_SOURCE_DIR}/src/client/client.cc)


message(STATUS "Filtered sources: ${hh_sources}")

find_package(Threads REQUIRED)

# 创建一个共享库 'hh_shared'，并将 hh_sources 中的源文件加入到库中
add_library(hh_shared SHARED ${hh_sources})

# 设置编译选项，使用自定义的 C++ 编译选项（通常是针对 C++17 标准的设置）
# target_compile_options(hh_shared PRIVATE -std=c++17) # 这行被注释掉了，可能需要根据需要启用
target_compile_options(hh_shared PRIVATE ${SRC_CXX_FLAGS})


# 链接线程库
target_link_libraries(hh_shared Threads::Threads redis++
    hiredis
    re2
    boost_locale)

find_library(REDIS_PLUS_PLUS_LIB redis++ PATHS ${REDIS_PLUS_PLUS_LIB_PATH})
target_link_options(hh_shared PRIVATE ${REDIS_PLUS_PLUS_LIB})

# 在 hh_shared 的 CMakeLists.txt 文件中确保链接这些库


# 设置链接选项，链接 redis++、hiredis 和 re2 库
target_link_options(hh_shared PRIVATE 
                        -lredis++     # 连接 redis++ 库
                        -lhiredis     # 连接 hiredis 库
                        -lre2         # 连接 re2 库
                        -lboost_locale
                        )


######################################################################################################################
# THIRD-PARTY SOURCES
######################################################################################################################

# 定义一个宏，用于添加第三方库
# 这个宏会接收一个库名 LIB_NAME，并根据该库名来递归查找其源代码文件（.cpp、.cc、.hpp、.h）
# 然后将这些源文件添加到一个共享库，并将该库链接到 'hh_shared' 库
macro(add_thirdparty_lib LIB_NAME)
    # 通过 GLOB_RECURSE 搜索第三方库目录下的所有源文件
    file(GLOB_RECURSE ${LIB_NAME}_sources
        ${PROJECT_SOURCE_DIR}/third_party/${LIB_NAME}/*.cpp
        ${PROJECT_SOURCE_DIR}/third_party/${LIB_NAME}/*.cc
        ${PROJECT_SOURCE_DIR}/third_party/${LIB_NAME}/*.hpp
        ${PROJECT_SOURCE_DIR}/third_party/${LIB_NAME}/*.h
        )
    # 创建第三方库的共享库，并将其源文件加入
    add_library(thirdparty_${LIB_NAME} SHARED ${${LIB_NAME}_sources})

    # 将第三方库链接到 'hh_shared' 库
    target_link_libraries(hh_shared thirdparty_${LIB_NAME})
endmacro()

# 添加 tinyxml2 第三方库
add_thirdparty_lib(tinyxml2)

# 如果有需要，也可以通过下面的代码添加 reactor 子目录（目前被注释掉）
# add_subdirectory(reactor)
    
