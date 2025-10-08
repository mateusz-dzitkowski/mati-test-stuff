cmake_minimum_required(VERSION 3.31)
project(redic C)

set(CMAKE_C_STANDARD 23)

add_executable(redic
        server.c
        client.c
        setup.c)
