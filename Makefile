.PHONY: init build run clean execute

BUILD_TYPE=Debug
GENERATOR="Unix Makefiles"

SHELL=/bin/bash

COMPILER_PATH=/usr/bin/x86_64-linux-gnu-g++-12

CMAKE_INIT=cmake -DCMAKE_GENERATOR:STRING=${GENERATOR} -DCMAKE_CXX_COMPILER:FILEPATH=${COMPILER_PATH}\
-DCMAKE_BUILD_TYPE:STRING=${BUILD_TYPE} ..

CONAN_PROFILE=default
CONAN_INIT=conan install .. --build=missing -s build_type=${BUILD_TYPE} --profile=${CONAN_PROFILE}

APP_NAME=prettyurl
CFG_PATH=/home/alex/Projects/C++/pretty-url/configs/app-config.json

init:
	mkdir -p ./build && cd ./build && ${CONAN_INIT} && ${CMAKE_INIT}

build:
	cd ./build && cmake --build .

execute:
	./bin/${BUILD_TYPE}/${APP_NAME} --config ${CFG_PATH}

run: build execute
	
clean:
	rm -rf ./build