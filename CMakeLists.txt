cmake_minimum_required(VERSION 3.25)
project(P2G4 C)

set(CMAKE_C_STANDARD 11)


add_executable(P2G4
        main.c sqlite3.c sqlite3.h auth.c auth.h)