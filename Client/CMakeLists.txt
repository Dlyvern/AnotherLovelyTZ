cmake_minimum_required(VERSION 3.15)
project(Client LANGUAGES C)

set(CMAKE_INSTALL_RPATH "$ORIGIN")

set(INCLUDE_DIR include)
set(SRC_DIR src)

file(GLOB_RECURSE PROJECT_HEADERS ${INCLUDE_DIR}/*.h)
file(GLOB_RECURSE PROJECT_SOURCES ${SRC_DIR}/*.c)

add_executable(${PROJECT_NAME}
        ${PROJECT_SOURCES}
        ${PROJECT_HEADERS}
)

target_include_directories(${PROJECT_NAME} PRIVATE
        ${INCLUDE_DIR}
)

target_link_libraries(${PROJECT_NAME} Protocol)

if(UNIX AND NOT APPLE)
    find_package(Threads REQUIRED)
    target_link_libraries(${PROJECT_NAME} Threads::Threads)
endif()

install(TARGETS ${PROJECT_NAME} DESTINATION .)
