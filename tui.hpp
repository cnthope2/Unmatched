#ifndef TUI_HPP
#define TUI_HPP

class Game;

class TUI
{
private:
    Game& game;

    void runMainMenu();
    void runGameDashboard();
    void runHelp();

public:
    explicit TUI(Game& game);
    void run();
};

#endif