cmake_minimum_required(VERSION 3.19)
project(DB_QT_Coursework)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Widgets Xml Core REQUIRED)
find_package(spdlog REQUIRED)

add_subdirectory(Libs/pqxx)
include_directories(Libs/pqxx/include)


add_executable(${PROJECT_NAME} main.cpp MainWindow.hxx settingsDialog.hxx Functions.hpp)

target_link_libraries(${PROJECT_NAME} Qt5::Widgets Qt5::Xml Qt5::Core pqxx spdlog::spdlog)