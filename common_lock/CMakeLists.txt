cmake_minimum_required(VERSION 3.0)

project(example)

# 如果代码需要支持C++11，就直接加上这句
#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# 如果想要生成的可执行文件拥有符号表，可以gdb调试，就直接加上这句
add_definitions("-Wall -g")

#include_directories(
#        ${PROJECT_SOURCE_DIR}/include
#)


# 生成可执行文件
#add_executable(${PROJECT_NAME} lock_metux.c )
#add_executable(spinlock lock_spin.c )
add_executable(${PROJECT_NAME} lock_atomic.c )

target_link_libraries(${PROJECT_NAME}
	pthread
	z
)
