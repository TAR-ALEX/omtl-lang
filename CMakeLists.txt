cmake_minimum_required(VERSION 3.16)
project(omtl-cc)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

if(WIN32)
  # see https://blog.kitware.com/create-dlls-on-windows-without-declspec-using-new-cmake-export-all-feature/
  set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif(WIN32)

# set(LLVM_DIR "vendor/llvm/usr/lib/llvm-14/cmake")
find_package(LLVM REQUIRED CONFIG )#PATHS "vendor/llvm/usr/lib/llvm-14/cmake" NO_DEFAULT_PATH
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})

add_definitions(${LLVM_DEFINITIONS})
include_directories(${PROJECT_SOURCE_DIR}/include/ ${PROJECT_SOURCE_DIR}/vendor/include/ ${PROJECT_SOURCE_DIR}/vendor-mod/include/ ${LLVM_INCLUDE_DIRS}) 
set(CMAKE_BUILD_WITH_INSTALL_RPATH on)
set(CMAKE_INSTALL_RPATH "./vendor/lib" "./lib")
link_directories(${PROJECT_SOURCE_DIR}/vendor/lib ${LLVM_DIR}../lib)

file(GLOB_RECURSE APP_SOURCES CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/src/*" "${PROJECT_SOURCE_DIR}/include/*")
file(GLOB_RECURSE VENDOR_SOURCES CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/vendor/src/*" "${PROJECT_SOURCE_DIR}/vendor/include/*" "${PROJECT_SOURCE_DIR}/vendor-mod/include/*")

separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})
llvm_map_components_to_libnames(llvm_libs ${LLVM_TARGETS_TO_BUILD} support core irreader codegen mc mcjit mcparser option)#

message(STATUS "LLVM_LIBS: ${llvm_libs}")

add_executable(${PROJECT_NAME} ${APP_SOURCES} ${VENDOR_SOURCES})
target_compile_options(${PROJECT_NAME} PRIVATE)
target_link_libraries(${PROJECT_NAME} pthread ${llvm_libs})