cmake_minimum_required(VERSION 3.27)

project(blockchain)

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
add_executable(${PROJECT_NAME} ${SRC_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_include_directories(${PROJECT_NAME} PRIVATE D:/C++/libs/asio_1_30_2/include)
target_include_directories(${PROJECT_NAME} PRIVATE D:/C++/libs/openssl/Lib/x64/Release/include)
