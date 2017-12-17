cmake_minimum_required(VERSION 3.0)
project(nife-x86)

set(SRC src/main.c src/exe.c src/disassemble.c src/common.c src/elf.c)
set(HEADERS include/exe.h include/common.h include/disassemble.h include/elf.h)

include_directories(include/)

add_executable(nife-x86 ${SRC} ${HEADERS})
