cmake_minimum_required(VERSION 3.24)
project(untitled3 C)

set(CMAKE_C_STANDARD 99)

add_executable(untitled3 src/main.c src/views/main_menus.h src/views/main_menus.c src/util/menu_manager.c src/util/menu_manager.h src/util/input.c src/util/input.h src/model/order.h src/model/client.h src/model/furniture.h src/model/admin.h src/util/array.h src/util/array.c src/model/state.c src/model/state.h)
