cmake_minimum_required(VERSION 3.14)
project(expedition LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

add_library(expedition_core
    src/core/room.cpp
    src/core/passage.cpp
    src/comm/action.cpp
    src/comm/response.cpp
    src/expedition/passage_data.cpp
    src/expedition/expedition_data.cpp
    src/expedition/generator.cpp
    src/expedition/expedition.cpp
    src/io/parser.cpp
    src/algo/dummy.cpp
)

target_include_directories(expedition_core PUBLIC include)

add_executable(expedition
    src/main.cpp
)
target_link_libraries(expedition PRIVATE expedition_core)
