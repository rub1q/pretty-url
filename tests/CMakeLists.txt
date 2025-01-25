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
  ../src/core/logging/base_logger.cpp
  ../src/core/encoding/base62_encoder.cpp
  ../src/core/utility/url_validator.cpp
  ../src/core/db/query_result_set.cpp
  ../src/core/net/http/route_matcher.cpp

  ../src/app/logging/details/registry.cpp  
  ../src/app/services/redirect_url_service.cpp
  ../src/app/services/url_shortener_service.cpp

  ../src/infra/net/http/response.cpp
  ../src/infra/net/http/request.cpp
  ../src/infra/net/http/route.cpp
  ../src/infra/net/http/router.cpp
  ../src/infra/db/auto_inc_id_generator.cpp
)

add_executable(${CMAKE_PROJECT_NAME} ${SOURCES}
  unit/core/repositories/url_repository_tests.cpp
  unit/core/encoding/base62_encoder_tests.cpp
  unit/core/utility/url_validator_tests.cpp
  unit/core/utility/string_utils_tests.cpp
  unit/core/db/db_session_manager_tests.cpp
  unit/core/db/query_result_set_tests.cpp
  unit/core/net/http/route_matcher_tests.cpp

  unit/app/services/redirect_url_service_tests.cpp
  unit/app/services/url_shortener_service_tests.cpp

  unit/infra/net/http/route_tests.cpp
  unit/infra/net/http/router_tests.cpp
)

target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC 
  "../include"
  "/home/alex/.conan2/p/b/fmtd62d03804a858/p/include/" #fmt
)

target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC Catch2::Catch2WithMain)

if (NOT ${STD_FORMAT_SUPPORT})
  target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC fmt::fmt)
endif()