cmake_minimum_required(VERSION 3.5)
project(LabSys)
set(CMAKE_AUTOMOC ON)

find_package(Qt5 REQUIRED COMPONENTS Widgets)

add_subdirectory(sqlite)
add_subdirectory(src)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE sqlite Qt5::Widgets Laboratory)

# target_compile_definitions(${PROJECT_NAME} PRIVATE QT_WIDGETS_LIB)
