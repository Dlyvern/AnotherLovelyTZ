cmake_minimum_required(VERSION 3.15)
project(qcustomplot LANGUAGES CXX)

find_package(Qt5 COMPONENTS Widgets PrintSupport REQUIRED)

add_library(qcustomplot STATIC
    qcustomplot/src/qcustomplot.cpp
    qcustomplot/include/qcustomplot.h
)

set_target_properties(qcustomplot PROPERTIES
    AUTOMOC ON
)

target_include_directories(qcustomplot PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/qcustomplot/include
)

target_link_libraries(qcustomplot PUBLIC
    Qt5::Widgets
    Qt5::PrintSupport
)
