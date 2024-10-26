cmake_minimum_required(VERSION 3.16)
project(omtl_cc)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_VERBOSE_MAKEFILE ON)

set(LLVM_DIR ${PROJECT_SOURCE_DIR}/vendor/llvm/lib/cmake/llvm)
find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

include_directories(${LLVM_INCLUDE_DIRS})
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})

# Find the libraries that correspond to the LLVM components
# that we wish to use
# llvm_map_components_to_libnames(llvm_libs support core irreader)
llvm_map_components_to_libnames(llvm_libs support core irreader ExecutionEngine OrcJIT nativecodegen)
message(STATUS "linking libs ${llvm_libs}")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

include_directories(${PROJECT_SOURCE_DIR}/include/ ${PROJECT_SOURCE_DIR}/vendor/include/ ${PROJECT_SOURCE_DIR}/vendor-mod/include/) 

set(CMAKE_BUILD_WITH_INSTALL_RPATH on)
set(CMAKE_INSTALL_RPATH "./vendor/lib" "./lib" "./vendor/llvm/lib")
link_directories(${PROJECT_SOURCE_DIR}/vendor/lib ${PROJECT_SOURCE_DIR}/vendor/llvm/lib)

file(GLOB_RECURSE APP_SOURCES CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/src/*" "${PROJECT_SOURCE_DIR}/include/*" "${PROJECT_SOURCE_DIR}/vendor-mod/include/*")
file(GLOB_RECURSE VENDOR_SOURCES CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/vendor/src/*" "${PROJECT_SOURCE_DIR}/vendor/include/*" "${PROJECT_SOURCE_DIR}/vendor-mod/include/*")
file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/test/*" "${PROJECT_SOURCE_DIR}/include/*" "${PROJECT_SOURCE_DIR}/vendor-mod/include/*")

add_executable(${PROJECT_NAME} ${APP_SOURCES} ${VENDOR_SOURCES} ./entry/main.cpp)
target_compile_options(${PROJECT_NAME} PRIVATE)
target_link_libraries(${PROJECT_NAME} pthread ${llvm_libs})


add_executable("${PROJECT_NAME}_TESTS" ${APP_SOURCES} ${VENDOR_SOURCES} ./entry/tests.cpp)
target_compile_options("${PROJECT_NAME}_TESTS" PRIVATE)
target_link_libraries("${PROJECT_NAME}_TESTS" pthread ${llvm_libs})
