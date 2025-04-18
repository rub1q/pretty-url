cmake_minimum_required(VERSION 3.22)

project(prettyurl
  VERSION 1.0.0.0
  LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (CMAKE_BUILD_TYPE STREQUAL Debug)
  if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m64 -Wall -Wextra -O0 -g3")
  endif()

  add_compile_definitions(__DEBUG__)
else()
  if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m64 -Wall -Wextra -O2")
  endif()
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}")

message(STATUS "Generator: '${CMAKE_GENERATOR}'")
message(STATUS "Compiler: '${CMAKE_CXX_COMPILER}'")
message(STATUS "Compiler version: '${CMAKE_CXX_COMPILER_VERSION}'")
message(STATUS "Build type: '${CMAKE_BUILD_TYPE}'")

include(${CMAKE_SOURCE_DIR}/cmake/create_version_file.cmake)
include(${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/generators/conan_toolchain.cmake)

create_version_file()

set(CONAN_DISABLE_CHECK_COMPILER ON)

set(CMAKE_THREAD_LIBS_INIT "-lpthread")
set(CMAKE_HAVE_THREADS_LIBRARY 1)
set(CMAKE_USE_WIN32_THREADS_INIT 0)
set(CMAKE_USE_PTHREADS_INIT 1)
set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(Threads REQUIRED)
find_package(RapidJSON REQUIRED)
find_package(spdlog REQUIRED)
find_package(Boost 1.78.0 EXACT REQUIRED)
find_package(libpqxx REQUIRED)
find_package(hiredis REQUIRED)

if (Boost_FOUND)
  add_compile_definitions(BOOST_BEAST_USE_STD_STRING_VIEW)
endif()

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
  src/main.cpp
  src/app.cpp

  src/core/logging/base_logger.cpp
  src/core/encoding/base62_encoder.cpp
  src/core/utility/url_validator.cpp
  src/core/db/query_result_set.cpp
  src/core/net/http/route_matcher.cpp

  src/app/logging/details/registry.cpp
  src/app/handlers/redirect_handler.cpp
  src/app/handlers/url_shortener_handler.cpp
  src/app/services/url_shortener_service.cpp
  src/app/services/redirect_url_service.cpp
  src/app/cli/cli_parser.cpp

  src/infra/logging/loggers/console_logger.cpp
  src/infra/logging/loggers/file_logger.cpp
  src/infra/net/http/request.cpp
  src/infra/net/http/response.cpp
  src/infra/net/http/route.cpp
  src/infra/net/http/router.cpp
  src/infra/db/repository/pg_url_repository_impl.cpp
  src/infra/db/auto_inc_id_generator.cpp
  src/infra/db/pg_session.cpp
  src/infra/db/pg_connect_string_builder.cpp  
  src/infra/caching/redis_cache.cpp    
  src/infra/config/json_config_parser.cpp
)

add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE 
  "${CMAKE_SOURCE_DIR}/include"
)
 
target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC 
  Threads::Threads
  boost::boost
  spdlog::spdlog
  rapidjson
  libpqxx::pqxx
  hiredis::hiredis
)

if (NOT ${STD_FORMAT_SUPPORT})
  target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC fmt::fmt)
endif()