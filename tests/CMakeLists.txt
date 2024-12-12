cmake_minimum_required(VERSION 3.22)

project(prettyurl_tests CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)

include(../build/${CMAKE_BUILD_TYPE}/generators/conan_toolchain.cmake)

find_package(Catch2 REQUIRED)

set(STD_FORMAT_SUPPORT ON)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  if (CMAKE_CXX_COMPILER_MAJOR_VERSION VERSION_LESS 13)
    message(STATUS "Compiler major version less than 13. fmt lib will be using")  
    set(STD_FORMAT_SUPPORT OFF)
  else()
    add_compile_definitions(HAS_STD_FORMAT_SUPPORT)
  endif()
endif()

if (NOT ${STD_FORMAT_SUPPORT})
  find_package(fmt REQUIRED)
endif()

set(SOURCES
  ../src/infra/net/http/response.cpp
  ../src/infra/net/http/request.cpp
  ../src/infra/net/http/route.cpp
  ../src/infra/net/http/router.cpp
  ../src/core/logging/base_logger.cpp
  ../src/app/logging/details/registry.cpp  

  unit/infra/net/http/route_tests.cpp
  unit/infra/net/http/router_tests.cpp
)

add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})

target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC 
  "../include"
  "/home/alex/.conan2/p/b/fmtd62d03804a858/p/include/"
)

target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC Catch2::Catch2WithMain)

if (NOT ${STD_FORMAT_SUPPORT})
  target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC fmt::fmt)
endif()