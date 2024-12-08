cmake_minimum_required(VERSION 3.22)

project(prettyurl_tests CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)

find_package(Catch2 REQUIRED)

set(SOURCES
  ../src/infra/net/http/response.cpp
  ../src/infra/net/http/request.cpp
  ../src/infra/net/http/route.cpp

  unit/infra/net/http/route_tests.cpp
)

add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})

target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC 
  "../include"
)

target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC Catch2::Catch2WithMain)