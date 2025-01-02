.PHONY: init build run execute clean

BUILD_TYPE=Debug
GENERATOR="Unix Makefiles"

SHELL=/bin/bash

COMPILER_PATH=/usr/bin/x86_64-linux-gnu-g++-12
CMAKE_INIT=cmake -DCMAKE_GENERATOR:STRING=${GENERATOR} -DCMAKE_CXX_COMPILER:FILEPATH=${COMPILER_PATH}\
-DCMAKE_BUILD_TYPE:STRING=${BUILD_TYPE} ..

APP_NAME=prettyurl_tests

init:
	(mkdir ./build || true) && cd ./build && ${CMAKE_INIT}

build:
	cd ./build && cmake --build .

execute:
	./build/${APP_NAME} --verbosity high --invisibles --durations yes

run: build execute

clean:
	rm -rf ./build