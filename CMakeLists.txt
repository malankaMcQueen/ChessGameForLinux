cmake_minimum_required(VERSION 3.26)
project(CourseWork)

set(CMAKE_CXX_STANDARD 17)

#set(CMAKE_EXE_LINKER_FLAGS "-static -static-libgcc")
set(SFML_STATIC_LIBRARIES TRUE)

set(SFML_DIR "${CMAKE_CURRENT_BINARY_DIR}/SFML-2.6.1-linux-gcc-64-bit/SFML-2.6.1/lib/cmake/SFML")

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake_modules")

find_package(SFML 2.6.1 REQUIRED COMPONENTS graphics)

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
        Work/Network.cpp
        Work/Network.h
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
        Work/Network.cpp
        Work/Network.h
)

target_link_libraries(PlayClient
        sfml-graphics)


target_link_libraries(PlayHost
        sfml-graphics
)
#target_link_libraries(PlayHost sfml-audio-d sfml-network-d sfml-graphics-d sfml-window-d sfml-system-d)



#target_link_libraries(OpenGL -lglew32 -lopengl32 -lfreeglut -lglfw3 -lgdi32)

