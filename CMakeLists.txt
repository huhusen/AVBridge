cmake_minimum_required(VERSION 3.25)
set(CMAKE_CXX_STANDARD 17)
set(root AVBridge)
#set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)
# 设置静态库文件目录
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

# 动态库文件目录
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

# 可执行文件目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

project(${root})
#定义源文件
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src sources)

#加载lib库
file(GLOB LIBNAMES ${CMAKE_CURRENT_SOURCE_DIR}/libs/debug/*.lib)

#引入项目头文件
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)


add_executable(${root} ${sources})
add_subdirectory(plugins/plugin-rtmp)
#链接第三方库
target_link_libraries(${root} PRIVATE ${LIBNAMES})