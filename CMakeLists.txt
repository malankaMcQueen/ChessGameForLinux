cmake_minimum_required(VERSION 3.26)
project(CourseWork)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXE_LINKER_FLAGS "-static -static-libgcc")
set(SFML_STATIC_LIBRARIES TRUE)

if (WIN32)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -mwindows")
endif()

add_executable(Work
        Work/main.cpp
        Work/Board.cpp
        Work/Board.h
        Work/Figure.h
        Work/Coordinates.h
        Work/Pawn.h
        Work/Knight.cpp
        Work/Knight.h
        Work/Bishop.h
        Work/Rook.cpp
        Work/Rook.h
        Work/Queen.h
        Work/King.h
        Work/ImageBoard.h
        Work/Figure.cpp
        Work/Pawn.cpp
        Work/Bishop.cpp
        Work/Header.h
        Work/Queen.cpp
        Work/King.cpp
        Work/Game.cpp
        Work/Game.h
        Work/StockFish.cpp
        Work/StockFish.h
        Work/ImageMenu.h
        Work/Exceptions.h
        Work/MyDeque.h
        Work/Algorithms.h
)


set(SFML_DIR "D:\\lib\\SFML-2.6.0-windows-gcc-13.1.0-mingw-64-bit\\SFML-2.6.0\\lib\\cmake\\SFML")

target_link_libraries(Work
        sfml-graphics)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake_modules")

find_package(SFML 2.6 REQUIRED graphics network audio window system )