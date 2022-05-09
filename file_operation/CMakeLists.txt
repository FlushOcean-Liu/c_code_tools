cmake_minimum_required(VERSION 3.0)

project(example)

# 如果代码需要支持C++11，就直接加上这句
#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# 如果想要生成的可执行文件拥有符号表，可以gdb调试，就直接加上这句
add_definitions("-Wall -g")


# 生成可执行文件
add_executable(${PROJECT_NAME} read_write_file.c )

target_link_libraries(${PROJECT_NAME}
	pthread
	z
)
