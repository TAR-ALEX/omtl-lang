cmake_minimum_required(VERSION 3.16)
project(omtl-cc)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

include_directories(${PROJECT_SOURCE_DIR}/include/ ${PROJECT_SOURCE_DIR}/vendor/include/ ${PROJECT_SOURCE_DIR}/vendor-mod/include/) 

set(CMAKE_BUILD_WITH_INSTALL_RPATH on)
set(CMAKE_INSTALL_RPATH "./vendor/lib" "./lib")
link_directories(${PROJECT_SOURCE_DIR}/vendor/lib)

file(GLOB_RECURSE APP_SOURCES CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/src/*" "${PROJECT_SOURCE_DIR}/include/*")
file(GLOB_RECURSE VENDOR_SOURCES CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/vendor/src/*" "${PROJECT_SOURCE_DIR}/vendor/include/*" "${PROJECT_SOURCE_DIR}/vendor-mod/include/*")

add_executable(${PROJECT_NAME} ${APP_SOURCES} ${VENDOR_SOURCES})
target_compile_options(${PROJECT_NAME} PRIVATE)
target_link_libraries(${PROJECT_NAME} pthread)