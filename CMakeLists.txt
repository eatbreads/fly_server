# 设置 CMake 最低版本和项目名称
cmake_minimum_required(VERSION 3.10)
project(MyWebProject)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置源文件和头文件目录
file(GLOB_RECURSE SRC_FILES 
    ${CMAKE_SOURCE_DIR}/main.cpp
    ${CMAKE_SOURCE_DIR}/utility/*.cpp
    ${CMAKE_SOURCE_DIR}/mapper/**/*.cpp
)

# 把当前目录添加到包含目录列表
include_directories(${CMAKE_CURRENT_SOURCE_DIR} 
                    ${CMAKE_CURRENT_SOURCE_DIR}/mapper/common
                    ${CMAKE_CURRENT_SOURCE_DIR}/mapper/entity
                    ${CMAKE_CURRENT_SOURCE_DIR}/mapper/sql
                    ${CMAKE_CURRENT_SOURCE_DIR}/mapper/util
                    ${CMAKE_CURRENT_SOURCE_DIR}/mapper/guard
                    ${CMAKE_CURRENT_SOURCE_DIR}/mapper/aop
                    ${CMAKE_CURRENT_SOURCE_DIR}/mapper/builder
                    ${CMAKE_CURRENT_SOURCE_DIR}/mapper
                    /usr/include/mysql
                    /usr/local/include/hiredis
                    )


#添加库文件搜索路径
link_directories(/usr/lib64/mysql)

# 设置可执行文件的输出路径为 CMakeLists.txt 所在的目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

# 添加可执行目标
add_executable(main ${SRC_FILES})

# 链接动态库
target_link_libraries(main 
    PRIVATE 
    pthread 
    libmysqlclient.so 
    ssl 
    crypto 
    hiredis
)
