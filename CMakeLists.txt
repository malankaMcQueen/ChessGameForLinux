cmake_minimum_required(VERSION 3.26)
project(CourseWork)

set(CMAKE_CXX_STANDARD 17)

set(SFML_DIR "${CMAKE_CURRENT_BINARY_DIR}/SFML-2.6.1-linux-gcc-64-bit\\SFML-2.6.1\\lib\\cmake\\SFML")

find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)

add_executable(PlayHost
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


add_executable(PlayClient
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

target_link_libraries(PlayHost
        sfml-graphics sfml-audio)


target_link_libraries(PlayClient
        sfml-graphics sfml-audio)