cmake_minimum_required(VERSION 3.23)
project(C_with_SQL C)

set(CMAKE_C_STANDARD 99)

add_executable(C_with_SQL main.c)
include_directories("C:/Program Files/MySQL/MySQL Server 8.0/include")
target_link_libraries(C_with_SQL "C:/Program Files/MySQL/MySQL Server 8.0/lib/libmysql.lib")

