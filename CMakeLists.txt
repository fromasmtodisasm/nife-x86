cmake_minimum_required(VERSION 3.0)
project(nife-x86)

set(SRC src/main.c src/exe.c)
set(HEADERS include/exe.h include/common.h)

include_directories(include/)

add_executable(nife-x86 ${SRC} ${HEADERS})
