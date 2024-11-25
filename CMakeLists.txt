
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

# 添加公共包含目录
include_directories(${CMAKE_CURRENT_SOURCE_DIR} 
                    ${CMAKE_CURRENT_SOURCE_DIR}/mapper/common
                    ${CMAKE_CURRENT_SOURCE_DIR}/mapper/entity
                    ${CMAKE_CURRENT_SOURCE_DIR}/mapper/sql
                    ${CMAKE_CURRENT_SOURCE_DIR}/mapper/util
                    ${CMAKE_CURRENT_SOURCE_DIR}/mapper/guard
                    ${CMAKE_CURRENT_SOURCE_DIR}/mapper/aop
                    ${CMAKE_CURRENT_SOURCE_DIR}/mapper/builder
                    ${CMAKE_CURRENT_SOURCE_DIR}/mapper
)

# 查找 Threads（线程库）
find_package(Threads REQUIRED)

# 查找 OpenSSL
find_package(OpenSSL REQUIRED)
if (OpenSSL_FOUND)
    include_directories(${OPENSSL_INCLUDE_DIR})
else()
    message(FATAL_ERROR "OpenSSL not found. Please install OpenSSL development package.")
endif()

# 查找 MySQL
find_package(MySQL)
if (MySQL_FOUND)
    message(STATUS "Using MySQL found by find_package")
    include_directories(${MySQL_INCLUDE_DIRS})
    set(MYSQL_LIBRARIES ${MySQL_LIBRARIES})
else()
    message(WARNING "MySQL not found using find_package. Falling back to manual configuration.")
    
    # 手动查找 MySQL
    find_library(MYSQL_LIBRARIES 
        NAMES libmysqlclient.so 
        PATHS /usr/lib /usr/lib64 /usr/lib64/mysql /usr/local/lib /usr/local/lib64/mysql /usr/lib/x86_64-linux-gnu NO_DEFAULT_PATH
    )
    find_path(MYSQL_INCLUDE_DIRS 
        NAMES mysql.h 
        PATHS /usr/include/mysql /usr/local/include/mysql NO_DEFAULT_PATH
    )
    if (MYSQL_LIBRARIES AND MYSQL_INCLUDE_DIRS)
        include_directories(${MYSQL_INCLUDE_DIRS})
        message(STATUS "Manually found MySQL: libraries=${MYSQL_LIBRARIES}, include_dirs=${MYSQL_INCLUDE_DIRS}")
    else()
        message(FATAL_ERROR "MySQL not found. Please install MySQL development package or specify paths manually.")
    endif()
endif()

# 查找 Hiredis（Redis 客户端）
find_library(HIREDIS_LIBRARIES 
    NAMES hiredis 
    PATHS /usr/lib /usr/lib64 /usr/local/lib /usr/local/lib64 /usr/lib/x86_64-linux-gnu NO_DEFAULT_PATH
)
find_path(HIREDIS_INCLUDE_DIRS 
    NAMES hiredis.h 
    PATHS /usr/include/hiredis /usr/local/include/hiredis NO_DEFAULT_PATH
)
if (HIREDIS_LIBRARIES AND HIREDIS_INCLUDE_DIRS)
    include_directories(${HIREDIS_INCLUDE_DIRS})
    message(STATUS "Found Hiredis: libraries=${HIREDIS_LIBRARIES}, include_dirs=${HIREDIS_INCLUDE_DIRS}")
else()
    message(FATAL_ERROR "Hiredis not found. Please install Hiredis library or specify paths manually.")
endif()

# 添加可执行目标
add_executable(main ${SRC_FILES})

# 设置库搜索路径和链接库
target_link_libraries(main 
    PRIVATE 
    Threads::Threads 
    ${OPENSSL_CRYPTO_LIBRARY} 
    ${OPENSSL_LIBRARIES}  # 添加 OpenSSL
    ${MYSQL_LIBRARIES} 
    ${HIREDIS_LIBRARIES}
)

# 添加库文件搜索路径
link_directories(${MYSQL_LIBRARIES} ${HIREDIS_LIBRARIES})

# 设置输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
