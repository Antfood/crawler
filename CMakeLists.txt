 cmake_minimum_required(VERSION 3.10)

 set(CMAKE_CXX_STANDARD 17)
 set(CMAKE_CXX_STANDARD_REQUIRED ON)

 project(crawler VERSION 0.1)

 file(GLOB_RECURSE SRC_FILES src/*.cpp)
 add_executable(crawler ${SRC_FILES})
 target_include_directories(crawler PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
