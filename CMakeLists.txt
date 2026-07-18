cmake_minimum_required(VERSION 4.0.0)

project(
    Unmached
    LANGUAGES CXX
    VERSION 1.0.0
)
include(FetchContent)

FetchContent_Declare(
    ftxui
    GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI.git
    GIT_TAG main
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
    game.cpp
    sherlock.cpp
    Watson.cpp
    discardpile.cpp
    tui.cpp
)
FetchContent_MakeAvailable(ftxui)

target_link_libraries(app
    PRIVATE
    ftxui::screen
    ftxui::dom
    ftxui::component
)