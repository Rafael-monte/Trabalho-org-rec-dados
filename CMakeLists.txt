cmake_minimum_required(VERSION 3.19)
project(trabalho_org_e_rec_dados C)

set(CMAKE_C_STANDARD 99)

add_executable(trabalho_org_e_rec_dados main.c files/flag-controller/flag-controller.h files/flag-controller/flag-controller.c files/file-handler/file-handler.h)