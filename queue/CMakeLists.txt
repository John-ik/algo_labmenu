cmake_minimum_required(VERSION 3.23)

add_library(queue)

target_sources(queue
    PRIVATE
        queue.c

    PUBLIC
        FILE_SET HEADERS
        FILES
            queue.h
)
