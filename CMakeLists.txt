cmake_minimum_required(VERSION 3.25)
set(CMAKE_CXX_STANDARD 17)
set(root EasyNvr)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)
project(${root})
#定义源文件
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src sources)

#加载lib库
file(GLOB LIBNAMES ${CMAKE_CURRENT_SOURCE_DIR}/libs/debug/*.lib)

#引入项目头文件
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
#引入插件的头文件
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/plugins)

add_executable(${root} ${sources})
add_subdirectory(plugins/plugin-rtmp)
#add_subdirectory(3rds/fmt EXCLUDE_FROM_ALL)
#链接第三方库
target_link_libraries(${root} PRIVATE ${LIBNAMES} plugin-rtmp)