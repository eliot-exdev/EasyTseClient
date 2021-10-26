cmake_minimum_required(VERSION 3.18)
project(tseclient)

find_package(Threads REQUIRED)
find_package(Boost 1.74 REQUIRED COMPONENTS system)

add_library(${PROJECT_NAME} STATIC
        src/Socket.cpp
        src/TseClient.cpp
        )
target_include_directories(${PROJECT_NAME}
        PUBLIC include
        PRIVATE ${Boost_INCLUDE_DIRS}
        )
target_link_libraries(${PROJECT_NAME}
        PUBLIC Threads::Threads
        )