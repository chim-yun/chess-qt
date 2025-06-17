cmake_minimum_required(VERSION 3.5)
project(chess-qt LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_CXX_STANDARD 17)

find_package(Qt5 COMPONENTS Widgets Sql REQUIRED)

add_executable(chess-qt
    src/main.cpp
    src/login.cpp
    src/chessgame.cpp
    resources/resources.qrc
)

target_include_directories(chess-qt PRIVATE src)
target_link_libraries(chess-qt Qt5::Widgets Qt5::Sql)

