#ifndef TUI_HPP
#define TUI_HPP
#include "game.hpp"
#include <string>
#include <utility>
#include <vector>
#include <array>
#include <memory>
#include <utility>
#include <iostream>
#include <ftxui/dom/canvas.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
class Game;

class TUI
{
private:
    Game &game;
    bool openHelp = false;
    bool returnToMenu = false;
    Character *attacker = nullptr;
    bool abilityMode = false;
    int selectedAttackCard = 0;
    Character *defender = nullptr;

    int selectedTarget = 0;

    int selectedDefenseCard = 0;
    int selectedAttackCardIndex = -1;

    int selectedSchemeCard = 0;

    int selectedAbility = 0;


    void runMainMenu();
    void runGameDashboard();
    void runHelp();

public:
    explicit TUI(Game &game);
    std::vector<std::string> attackCards;
    std::vector<int> attackCardIndexes;
    std::vector<std::string> targets;
    std::vector<Character *> targetPointers;
    std::vector<std::string> defenseCards;
    std::vector<int> defenseCardIndexes;
    std::vector<std::string> schemeCards;
    std::vector<int> schemeCardIndexes;
    std::vector<std::string> abilityItems;
    void run();
};

#endif