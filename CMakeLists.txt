cmake_minimum_required(VERSION 4.0.0)

project(
    Unmached
    LANGUAGES CXX
    VERSION 1.0.0
)

add_executable(app
    main.cpp
    character.cpp
    dracula.cpp
    sisters.cpp
    card.cpp
    deck.cpp
    hand.cpp
    space.cpp
    board.cpp
)