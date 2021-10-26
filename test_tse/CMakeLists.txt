cmake_minimum_required(VERSION 3.18)
project(test_tse)

add_executable(${PROJECT_NAME}
        src/main.cpp
        )
target_link_libraries(${PROJECT_NAME}
        PRIVATE tseclient
        )