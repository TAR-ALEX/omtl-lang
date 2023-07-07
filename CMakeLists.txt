cmake_minimum_required(VERSION 3.16)
project(omtl-cc)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_VERBOSE_MAKEFILE ON)

set(LLVM_BUILD_LLVM_DYLIB ON)
set(LLVM_TARGETS_TO_BUILD "X86")
add_subdirectory(vendor/llvm-project/llvm)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})

add_definitions(${LLVM_DEFINITIONS})
# set(LLVM_INCLUDE_DIRS "./vendor/llvm-project/llvm/include")

include_directories(${PROJECT_SOURCE_DIR}/include/ ${PROJECT_SOURCE_DIR}/vendor/include/ ${PROJECT_SOURCE_DIR}/vendor-mod/include/) 
include_directories(${PROJECT_SOURCE_DIR}/vendor/llvm-project/llvm/include)
include_directories(${PROJECT_BINARY_DIR}/vendor/llvm-project/llvm/include)

set(CMAKE_BUILD_WITH_INSTALL_RPATH on)
set(CMAKE_INSTALL_RPATH "./vendor/lib" "./lib" "./build/vendor/llvm-project/llvm/lib")
link_directories(${PROJECT_SOURCE_DIR}/vendor/lib ${LLVM_DIR}../lib ./build/vendor/llvm-project/llvm/lib)

file(GLOB_RECURSE APP_SOURCES CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/src/*" "${PROJECT_SOURCE_DIR}/include/*")
file(GLOB_RECURSE VENDOR_SOURCES CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/vendor/src/*" "${PROJECT_SOURCE_DIR}/vendor/include/*" "${PROJECT_SOURCE_DIR}/vendor-mod/include/*")

separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})
llvm_map_components_to_libnames(llvm_libs ${LLVM_TARGETS_TO_BUILD} support core irreader codegen mc mcjit mcparser option)

#execute_process(COMMAND llvm-config --libs all
# OUTPUT_VARIABLE llvm_libraries)

message(STATUS "LLVM_LIBS: ${llvm_libs}")

add_executable(${PROJECT_NAME} ${APP_SOURCES} ${VENDOR_SOURCES})
target_compile_options(${PROJECT_NAME} PRIVATE)
target_link_libraries(${PROJECT_NAME} pthread libLLVM-16.so)