cmake_minimum_required(VERSION 3.29)
project(StartMenuToPTRun)

set(CMAKE_CXX_STANDARD 26)

add_executable(StartMenuToPTRun main.cpp
        KeyboardHook.h
        KeyboardHook.cpp
        Logger.cpp
        Logger.h)
