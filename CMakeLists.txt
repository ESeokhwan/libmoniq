cmake_minimum_required(VERSION 3.14)
project(moniq LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE MONITOR_SOURCES
    src/libmoniq/*.cpp
)
file(GLOB_RECURSE MONITOR_HEADERS
    src/libmoniq/*.h
    src/libmoniq/*.hpp
)

add_library(moniq STATIC
    ${MONITOR_SOURCES}
    ${MONITOR_HEADERS}
)

target_include_directories(moniq PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    $<INSTALL_INTERFACE:include>
)

target_compile_options(moniq PRIVATE -Wall -Wextra -Werror)

install(TARGETS moniq
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

install(DIRECTORY src/libmoniq/
    DESTINATION include/libmoniq
    FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp"
)