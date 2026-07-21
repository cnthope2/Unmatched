#include "tui.hpp"
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

using namespace ftxui;

namespace
{

    std::string drawSpace(int id)
    {
        if (id < 10)
        {
            return "[0" + std::to_string(id) + "]";
        }

        return "[" + std::to_string(id) + "]";
    }
    Element spaceElement(int id)
    {
        return text(drawSpace(id)) |
               bold |
               center |
               size(WIDTH, EQUAL, 6);
    }

    Element renderHeader()
    {
        return vbox({
            text("UNMATCHED") |
                bold |
                center |
                color(Color::Yellow),

            text("Dracula vs Sherlock Holmes") |
                center,
        });
    }
    std::string attackRangeToString(AttackRange range)
    {
        return range == AttackRange::Melee ? "Melee" : "Ranged";
    }
    Element renderPlayerPanel(
        const std::string &title,
        Color titleColor,
        int health,
        int maxHealth,
        int position,
        int movement,
        const std::string &range,
        int handSize,
        int deckSize,
        int discardSize,
        const std::vector<std::string> &fighters)
    {
        Elements content = {
            text(title) | bold | center | color(titleColor),

            separator(),

            text("HP       : " +
                 std::to_string(health) +
                 " / " +
                 std::to_string(maxHealth)),

            text("Position : " +
                 std::to_string(position)),

            text("Movement : " +
                 std::to_string(movement)),

            text("Range    : " + range),

            separator(),

            text("Hand     : " +
                 std::to_string(handSize)),

            text("Deck     : " +
                 std::to_string(deckSize)),

            text("Discard  : " +
                 std::to_string(discardSize)),

            separator(),

            text("FIGHTERS") |
                bold |
                color(titleColor),
        };

        for (const std::string &fighter : fighters)
        {
            content.push_back(text(fighter));
        }

        return vbox(std::move(content)) |
               border |
               size(WIDTH, EQUAL, 28);
    }
    struct FighterMarker
    {
        std::string text;
        Color color = Color::White;
    };

    FighterMarker getFighterMarker(Game &game, int spaceId)
    {
        FighterMarker marker;

        auto checkCharacter = [&](Character *character)
        {
            if (character == nullptr)
                return;

            if (character->isAlive() &&
                character->getPosition() == spaceId)
            {
                if (character->getName().find("Dracula") != std::string::npos)
                {
                    marker.text += "D";
                    marker.color = Color::Red;
                }
                else if (character->getName().find("Sherlock") != std::string::npos)
                {
                    marker.text += "S";
                    marker.color = Color::Blue;
                }
            }

            std::vector<Character *> sidekicks =
                character->getSidekicks();

            for (size_t i = 0; i < sidekicks.size(); i++)
            {
                Character *sidekick = sidekicks[i];

                if (sidekick == nullptr)
                    continue;

                if (!sidekick->isAlive())
                    continue;

                if (sidekick->getPosition() != spaceId)
                    continue;

                std::string name =
                    sidekick->getName();

                if (name.find("Watson") != std::string::npos)
                {
                    marker.text += "W";
                    marker.color = Color::Cyan;
                }
                else
                {
                    marker.text += char('A' + i);
                    marker.color = Color::Magenta;
                }
            }
        };

        checkCharacter(game.getCurrentPlayer());
        checkCharacter(game.getOpponent());

        return marker;
    }

}
Element renderBoardPanel(Game &game)
{
    Canvas boardCanvas(110, 56);

    struct P
    {
        int x;
        int y;
    };

    P p[33] = {
        {0, 0},

        {68, 26},
        {60, 40},
        {90, 34},
        {64, 48},
        {54, 22},
        {42, 22},
        {42, 34},
        {54, 34},
        {75, 34},
        {95, 42},
        // 10
        {90, 51},
        {52, 44},
        {36, 46},
        {24, 49},
        {17, 41},
        {18, 30},
        {31, 25},

        {3, 24},
        {13, 18},
        {20, 14},
        // 20
        {3, 8},
        {17, 6},
        {34, 11},
        {3, 1},
        {34, 1},

        {49, 2},
        {60, 8},
        {72, 2},
        {69, 16},
        {91, 11},
        {83, 25},
        {104, 10}};

    std::vector<std::pair<int, int>> edges =
        {
            {1, 2}, {1, 3}, {1, 5}, {1, 8}, {1, 9}, {1, 29}, {2, 4}, {3, 4}, {3, 31}, {4, 10}, {5, 6}, {6, 7}, {6, 17}, {7, 8}, {7, 12}, {7, 13}, {8, 9}, {8, 12}, {9, 10}, {9, 11}, {10, 11}, {12, 13}, {13, 14}, {14, 15}, {15, 16}, {16, 17}, {16, 18}, {18, 19}, {18, 21}, {19, 20}, {20, 22}, {20, 23}, {21, 22}, {21, 24}, {23, 25}, {23, 26}, {24, 25}, {26, 27}, {26, 28}, {27, 28}, {27, 29}, {28, 29}, {29, 30}, {29, 31}, {30, 31}, {30, 32}};

    for (auto &e : edges)
    {
        boardCanvas.DrawPointLine(
            p[e.first].x + 2,
            p[e.first].y,
            p[e.second].x + 2,
            p[e.second].y,
            Color::GrayDark);
    }
    for (int i = 1; i <= 32; i++)
    {
        std::string number =
            (i < 10)
                ? "0" + std::to_string(i)
                : std::to_string(i);

        boardCanvas.DrawText(
            p[i].x - 2,
            p[i].y,
            "[" + number + "]",
            Color::White);

        FighterMarker marker =
            getFighterMarker(game, i);

        if (!marker.text.empty())
        {
            boardCanvas.DrawText(
                p[i].x + 4,
                p[i].y,
                marker.text,
                marker.color);
        }
    }
    return vbox({text("BASKERVILLE MANOR") | bold | center | color(Color::Green),

                 separator(),

                 canvas(boardCanvas) | flex,

                 separator(),

                 hbox({
                     text("D ") | color(Color::Red) | bold,
                     text("Dracula   "),

                     text("S ") | color(Color::Blue) | bold,
                     text("Sherlock   "),

                     text("W ") | color(Color::Cyan) | bold,
                     text("Watson   "),

                     text("A/B/C ") | color(Color::Magenta) | bold,
                     text("Sisters"),
                 }) | center}) |
           border | size(WIDTH, EQUAL, 62) | flex;
}

Element drawSpaceElement(int id)
{
    return text(drawSpace(id)) |
           bold |
           center |
           flex;
}

Element renderHandPanel(const Component &handMenu)
{
    return vbox({
               text("CURRENT HAND") |
                   bold |
                   center |
                   color(Color::Cyan),

               separator(),

               handMenu->Render() |
                   vscroll_indicator |
                   frame |
                   flex,
           }) |
           border |
           flex;
}

Element renderActionPanel(
    const Component &actionMenu)
{
    return vbox({
               text("ACTIONS") |
                   bold |
                   center |
                   color(Color::Yellow),

               separator(),

               actionMenu->Render() | vscroll_indicator |
                   frame |
                   size(HEIGHT, EQUAL, 6),

               separator(),

               text("Arrow keys: Navigate") |
                   dim |
                   center,

               text("Enter: Select") |
                   dim |
                   center,
           }) |
           border |
           size(WIDTH, EQUAL, 31);
}
Element renderLogPanel(const std::vector<std::string> &actionlogs)
{
    Elements logs;

    logs.push_back(
        text("ACTION LOG") |
        bold |
        color(Color::Yellow));

    logs.push_back(separator());
    size_t start = 0;

    if (actionlogs.size() > 5)
        start = actionlogs.size() - 5;

    for (size_t i = start; i < actionlogs.size(); ++i)
    {
        logs.push_back(text("> " + actionlogs[i]));
    }
    return vbox(std::move(logs)) |
           yframe |
           vscroll_indicator |
           border |
           flex;
}
Element renderSelectionPanel(
    const std::string &selectedAction, int actionsRemaining)
{
    return vbox({
               text("SELECTION INFO") |
                   bold |
                   color(Color::Magenta),

               separator(),

               text(
                   "Selected action: " +
                   selectedAction),

               text("Remaining actions: " + std::to_string(actionsRemaining)),

           }) |
           border |
           size(WIDTH, EQUAL, 30);
}

TUI::TUI(Game &game) : game(game)
{
}
void TUI::run()
{
    runMainMenu();
}

void TUI::runMainMenu()
{
    auto screen =
        ScreenInteractive::Fullscreen();

    std::vector<std::string> entries = {
        "Play",
        "Help",
        "Exit",
    };

    int selected = 0;

    bool playSelected = false;
    bool helpSelected = false;

    MenuOption option;

    option.on_enter = [&]
    {
        if (selected == 0)
        {
            playSelected = true;
        }
        else if (selected == 1)
        {
            helpSelected = true;
        }

        screen.ExitLoopClosure()();
    };

    Component menu =
        Menu(
            &entries,
            &selected,
            option);

    Component renderer =
        Renderer(
            menu,
            [&]
            {
                Element menuPanel =
                    vbox({
                        text("UNMATCHED") |
                            bold |
                            center |
                            color(Color::Yellow),

                        text(
                            "Dracula vs Sherlock Holmes") |
                            center,

                        text("Baskerville Manor") |
                            dim |
                            center,

                        separator(),

                        menu->Render() |
                            frame |
                            size(WIDTH, EQUAL, 30),

                        separator(),

                        text(
                            "Use Arrow Keys and Enter") |
                            dim |
                            center,
                    }) |
                    border |
                    size(WIDTH, EQUAL, 46);

                return vbox({
                           filler(),
                           menuPanel | center,
                           filler(),
                       }) |
                       flex;
            });

    screen.Loop(renderer);

    if (playSelected)
    {
        runGameDashboard();
    }
    else if (helpSelected)
    {
        runHelp();
        runMainMenu();
    }
}

void TUI::runHelp()
{
    auto screen =
        ScreenInteractive::Fullscreen();

    Component backButton =
        Button(
            "Back",
            screen.ExitLoopClosure());

    Component renderer =
        Renderer(
            backButton,
            [&]
            {
                Element helpContent =
                    vbox({
                        text("HELP") |
                            bold |
                            center |
                            color(Color::Yellow),

                        separator(),

                        text("CONTROLS") |
                            bold,

                        text(
                            "Arrow Keys - Navigate menus"),

                        text(
                            "Enter      - Select"),

                        text(
                            "Escape     - Return or cancel"),

                        separator(),

                        text("ACTIONS") |
                            bold,

                        text(
                            "Maneuver - Draw a card and move"),

                        text(
                            "Attack - Select fighter and target"),

                        text(
                            "Scheme - Play a Scheme card"),

                        text(
                            "Ability - Use Dracula's ability"),

                        separator(),

                        text("OBJECTIVE") |
                            bold,

                        text(
                            "Defeat the opposing Hero."),

                        separator(),

                        backButton->Render() |
                            center,
                    }) |
                    border |
                    size(WIDTH, EQUAL, 62);

                return vbox({
                           filler(),
                           helpContent | center,
                           filler(),
                       }) |
                       flex;
            });

    screen.Loop(renderer);
}

void TUI::runGameDashboard()
{
    auto screen =
        ScreenInteractive::Fullscreen();

    std::vector<std::string> actions = {
        "Maneuver",
        "Attack",
        "Scheme",
        "Use Ability",
        "Help",
        "Back to Main Menu",
    };

    int selectedAction = 0;
    int selectedFighter = 0;
    int selectedDestination = 0;
    int activeMenu = 0;
    bool atackMood = false;

    Character *fighterToMove = nullptr;

    std::vector<std::string> fighters;
    std::vector<Character *> fighterPointers;
    std::vector<std::string> destinations;
    std::vector<std::string> handItems;
    int selectedHand = 0;

    Component handMenu = Menu(&handItems, &selectedHand);

    for (int i = 1; i <= 32; ++i)
    {
        destinations.push_back("Space " + std::to_string(i));
    }

    destinations.push_back("Cancel");
    auto refreshFighterMenu = [&]()
    {
        fighters.clear();
        fighterPointers.clear();
        selectedFighter = 0;

        Character *current = game.getCurrentPlayer();

        if (selectedHand >= static_cast<int>(handItems.size()))
        {
            selectedHand =
                static_cast<int>(handItems.size()) - 1;
        }

        if (current == nullptr)
        {
            fighters.push_back("Cancel");
            return;
        }

        if (current->isAlive())
        {
            fighters.push_back(
                current->getName() +
                " - Space " +
                std::to_string(current->getPosition()));

            fighterPointers.push_back(current);
        }

        for (Character *sidekick : current->getSidekicks())
        {
            if (sidekick != nullptr &&
                sidekick->isAlive())
            {
                fighters.push_back(
                    sidekick->getName() +
                    " - Space " +
                    std::to_string(sidekick->getPosition()));

                fighterPointers.push_back(sidekick);
            }
        }

        fighters.push_back("Finish");
    };

    bool openHelp = false;
    bool returnToMenu = false;
    Character *attacker = nullptr;
    std::vector<std::string> attackCards;
    std::vector<int> attackCardIndexes;
    int selectedAttackCard = 0;
    Character *defender = nullptr;
    std::vector<std::string> targets;
    std::vector<Character *> targetPointers;
    int selectedTarget = 0;
    std::vector<std::string> defenseCards;
    std::vector<int> defenseCardIndexes;
    int selectedDefenseCard = 0;
    int selectedAttackCardIndex = -1;
    std::vector<std::string> schemeCards;
    std::vector<int> schemeCardIndexes;
    int selectedSchemeCard = 0;
    std::vector<std::string> abilityItems;
    int selectedAbility = 0;
    MenuOption actionOption;
    MenuOption targetOption;

    auto refreshDefenseCardMenu = [&]()
    {
        defenseCards.clear();
        defenseCardIndexes.clear();

        selectedDefenseCard = 0;

        Character *owner =
            game.getOpponent();

        if (owner == nullptr)
            return;

        const auto &cards =
            owner->getHand().getCards();

        for (int i = 0;
             i < static_cast<int>(cards.size());
             ++i)
        {
            const Card &card = cards[i];

            bool defenseType =
                card.getType() == CardType::Defense ||
                card.getType() == CardType::Versatile;

            if (defenseType &&
                game.canFighterUseCard(defender, card))
            {
                defenseCards.push_back(
                    card.getName() +
                    " - DEF " +
                    std::to_string(
                        card.getDefenseValue()));

                defenseCardIndexes.push_back(i);
            }
        }

        defenseCards.push_back("Skip Defense");
    };

    MenuOption attackCardOption;

    attackCardOption.on_enter = [&]
    {
        if (selectedAttackCard ==
            static_cast<int>(attackCards.size()) - 1)
        {
            defender = nullptr;
            activeMenu = 3;
            return;
        }
        selectedAttackCardIndex = attackCardIndexes[selectedAttackCard];
        refreshDefenseCardMenu();
        activeMenu = 5;
    };

    Component attackCardMenu =
        Menu(
            &attackCards,
            &selectedAttackCard,
            attackCardOption);

    MenuOption defenseCardOption;

    defenseCardOption.on_enter = [&]
    {
        int defenseIndex = -1;

        bool skip =
            selectedDefenseCard ==
            static_cast<int>(
                defenseCards.size()) -
                1;

        if (!skip)
        {
            defenseIndex =
                defenseCardIndexes[selectedDefenseCard];
        }

        game.resolveCombatFromTUI(
            attacker,
            defender,
            selectedAttackCardIndex,
            defenseIndex);

        attacker = nullptr;
        defender = nullptr;

        atackMood = false;

        activeMenu = 0;
    };

    Component defenseCardMenu =
        Menu(
            &defenseCards,
            &selectedDefenseCard,
            defenseCardOption);

    auto refreshTargetMenu = [&]()
    {
        targets.clear();
        targetPointers.clear();
        selectedTarget = 0;

        if (attacker == nullptr)
        {
            return;
        }

        std::vector<Character *> list =
            game.getAttackableTargets(attacker);

        for (Character *target : list)
        {
            targets.push_back(
                target->getName() +
                " - Space " +
                std::to_string(target->getPosition()));

            targetPointers.push_back(target);
        }

        targets.push_back("Cancel");
    };
    auto refreshAttackCardMenu = [&]()
    {
        attackCards.clear();
        attackCardIndexes.clear();
        selectedAttackCard = 0;

        if (attacker == nullptr)
            return;

        Character *owner = game.getCurrentPlayer();

        const auto &cards =
            owner->getHand().getCards();

        for (int i = 0;
             i < static_cast<int>(cards.size());
             ++i)
        {
            const Card &card = cards[i];

            bool attackType =
                card.getType() == CardType::Attack ||
                card.getType() == CardType::Versatile;

            if (attackType &&
                game.canFighterUseCard(attacker, card))
            {
                attackCards.push_back(
                    card.getName() +
                    " - ATK " +
                    std::to_string(card.getAttackValue()));

                attackCardIndexes.push_back(i);
            }
        }

        attackCards.push_back("Cancel");
    };

    auto refreshSchemeCardMenu = [&]()
    {
        schemeCards.clear();
        schemeCardIndexes.clear();
        selectedSchemeCard = 0;

        Character *current = game.getCurrentPlayer();

        if (current == nullptr)
        {
            schemeCards.push_back("Cancel");
            return;
        }

        const auto &cards = current->getHand().getCards();

        for (int i = 0; i < static_cast<int>(cards.size()); ++i)
        {
            const Card &card = cards[i];

            if (card.getType() != CardType::Scheme)
            {
                continue;
            }

            schemeCards.push_back(card.getName());

            schemeCardIndexes.push_back(i);
        }

        if (schemeCardIndexes.empty())
        {
            schemeCards.push_back("No playable Scheme cards");
        }

        schemeCards.push_back("Cancel");
    };

    MenuOption schemeCardOption;

    schemeCardOption.on_enter = [&]()
    {
        int cancelIndex = static_cast<int>(schemeCards.size()) - 1;

        if (selectedSchemeCard == cancelIndex)
        {
            activeMenu = 0;
            return;
        }

        if (schemeCardIndexes.empty())
        {
            activeMenu = 0;
            return;
        }

        if (selectedSchemeCard < 0 || selectedSchemeCard >= static_cast<int>(schemeCardIndexes.size()))
        {
            activeMenu = 0;
            return;
        }

        int handIndex = schemeCardIndexes[selectedSchemeCard];

        game.playSchemeFromTUI(game.getCurrentPlayer(), handIndex);
        activeMenu = 0;
    };

    Component schemeCardMenu = Menu(&schemeCards, &selectedSchemeCard, schemeCardOption);
    MenuOption abilityOption;

    abilityOption.on_enter = [&]()
    {
        Character *current = game.getCurrentPlayer();

        if (current == nullptr)
            return;

        if (selectedAbility == 1)
        {
            activeMenu = 0;
            return;
        }

        game.useCharacterAbility(current);

        activeMenu = 0;
    };

    Component abilityMenu = Menu(&abilityItems, &selectedAbility, abilityOption);
    targetOption.on_enter = [&]
    {
        if (selectedTarget == static_cast<int>(targets.size()) - 1)
        {
            atackMood = false;
            attacker = nullptr;
            activeMenu = 0;
            return;
        }

        defender = targetPointers[selectedTarget];

        refreshAttackCardMenu();

        activeMenu = 4;
    };

    Component targetMenu =
        Menu(
            &targets,
            &selectedTarget,
            targetOption);
    actionOption.on_enter = [&]
    {
        Character *current =
            game.getCurrentPlayer();

        if (selectedAction == 0)
        {
            if (game.beginManeuver())
            {
                refreshFighterMenu();
                fighterToMove = nullptr;
                activeMenu = 1;
            }

            return;
        }
        else if (selectedAction == 1)
        {
            atackMood = true;
            refreshFighterMenu();
            activeMenu = 1;
            return;
        }
        else if (selectedAction == 2)
        {
            refreshSchemeCardMenu();
            activeMenu = 6;
            return;
        }
        else if (selectedAction == 3)
        {
            abilityItems.clear();

            if (current->getName() == "Dracula")
            {
                abilityItems.push_back("Feed");
            }
            else if (current->getName() == "Sherlock Holmes")
            {
                abilityItems.push_back("Detective Ability");
            }

            abilityItems.push_back("Cancel");

            selectedAbility = 0;
            activeMenu = 7;
        }
        else if (selectedAction == 4)
        {
            openHelp = true;
            screen.ExitLoopClosure()();
        }
        else if (selectedAction == 5)
        {
            returnToMenu = true;
            screen.ExitLoopClosure()();
        }
    };

    Component actionMenu =
        Menu(
            &actions,
            &selectedAction,
            actionOption);
    MenuOption fighterOption;
    fighterOption.on_enter = [&]
    {
        if (selectedFighter ==
            static_cast<int>(fighters.size()) - 1)
        {
            fighterToMove = nullptr;

            if (atackMood)
            {
                atackMood = false;
                attacker = nullptr;
            }
            else
            {
                game.finishManeuver();
            }

            activeMenu = 0;
            return;
        }

        if (selectedFighter < 0 ||
            selectedFighter >=
                static_cast<int>(fighterPointers.size()))
        {
            return;
        }

        if (atackMood)
        {
            attacker = fighterPointers[selectedFighter];
            refreshTargetMenu();
            activeMenu = 3;
            return;
        }

        fighterToMove =
            fighterPointers[selectedFighter];

        selectedDestination = 0;
        activeMenu = 2;
    };
    Component fighterMenu =
        Menu(
            &fighters,
            &selectedFighter,
            fighterOption);

    MenuOption destinationOption;

    destinationOption.on_enter = [&]
    {
        if (selectedDestination == 32)
        {
            fighterToMove = nullptr;
            activeMenu = 1;
            return;
        }

        if (fighterToMove == nullptr)
        {
            activeMenu = 0;
            return;
        }

        int destination =
            selectedDestination + 1;
        bool moved =
            game.moveDuringManeuver(
                fighterToMove,
                destination);
        if (moved)
        {
            fighterToMove = nullptr;

            refreshFighterMenu();
            activeMenu = 1;
        }
    };

    Component destinationMenu =
        Menu(
            &destinations,
            &selectedDestination,
            destinationOption);
    Component menuContainer =
        Container::Tab(
            {actionMenu,
             fighterMenu,
             destinationMenu,
             targetMenu,
             attackCardMenu,
             defenseCardMenu,
             schemeCardMenu,
             abilityMenu},
            &activeMenu);
    Component mainContainer =
        Container::Vertical({
            menuContainer,
            handMenu,
        });

    Component renderer =
        Renderer(
            mainContainer,
            [&]
            {
                Character *current = game.getCurrentPlayer();
                if (game.isGameOver())
                {
                    Character *winner = game.getWinner();

                    return vbox({
                        filler(),
                        window(
                            text("GAME OVER") | bold | center,
                            vbox({
                                text(winner->getName() + " Wins!") | center,
                                separator(),
                                text("Press ESC to Exit") | center,
                            })),
                        filler(),
                    });
                }
                handItems.clear();

                if (current != nullptr)
                {
                    const auto &cards = current->getHand().getCards();

                    for (const auto &card : cards)
                    {
                        handItems.push_back(card.getName());
                    }
                }

                if (handItems.empty())
                {
                    handItems.push_back("No cards in hand.");
                }

                Character *opponent =
                    game.getOpponent();

                if (current == nullptr ||
                    opponent == nullptr)
                {
                    return text("Game players are not available.") |
                           center |
                           border;
                }

                Character *dracula = nullptr;
                Character *sherlock = nullptr;

                if (current->getName() == "Dracula")
                {
                    dracula = current;
                    sherlock = opponent;
                }
                else
                {
                    sherlock = current;
                    dracula = opponent;
                }

                std::vector<std::string>
                    draculaFighters;
                draculaFighters.push_back(dracula->getName());
                draculaFighters.push_back("Space: " + std::to_string(dracula->getPosition()));

                for (Character *sidekick :
                     dracula->getSidekicks())
                {
                    if (sidekick != nullptr)
                    {
                        draculaFighters.push_back(sidekick->getName() + "Sp: " + std::to_string(sidekick->getPosition()));
                    }
                }

                std::vector<std::string>
                    sherlockFighters;

                sherlockFighters.push_back(sherlock->getName());
                sherlockFighters.push_back("Space: " + std::to_string(sherlock->getPosition()));
                sherlockFighters.push_back("");

                for (Character *sidekick :
                     sherlock->getSidekicks())
                {
                    if (sidekick != nullptr)
                    {
                        sherlockFighters.push_back(sidekick->getName());
                        sherlockFighters.push_back("Space: " + std::to_string(sidekick->getPosition()));
                        sherlockFighters.push_back("");
                    }
                }

                Color currentColor =
                    current->getName() == "Dracula"
                        ? Color::Red
                        : Color::Blue;

                Element header =
                    renderHeader();

                Element currentTurn =
                    hbox({text(" CURRENT TURN: "),

                          text(current->getName()) |
                              bold |
                              color(currentColor),

                          text(
                              " | Remaining actions:  " + std::to_string(game.getActionsRemaining()) + " ")}) |
                    center |
                    border;

                Element draculaPanel =
                    renderPlayerPanel(
                        dracula->getName(),
                        Color::Red,

                        dracula->getHealth(),
                        dracula->getMaxHealth(),

                        dracula->getPosition(),
                        dracula->getMovement(),

                        attackRangeToString(
                            dracula->getAttackRange()),

                        dracula->getHandSize(),
                        dracula->getDeck().size(),
                        dracula->getDiscardPile().size(),

                        draculaFighters) |
                    size(WIDTH, EQUAL, 28);

                Element boardPanel =
                    renderBoardPanel(game) |
                    size(WIDTH, EQUAL, 60);

                Element sherlockPanel =
                    renderPlayerPanel(
                        sherlock->getName(),
                        Color::Blue,

                        sherlock->getHealth(),
                        sherlock->getMaxHealth(),

                        sherlock->getPosition(),
                        sherlock->getMovement(),

                        attackRangeToString(
                            sherlock->getAttackRange()),

                        sherlock->getHandSize(),
                        sherlock->getDeck().size(),
                        sherlock->getDiscardPile().size(),

                        sherlockFighters) |
                    size(WIDTH, EQUAL, 28);

                Element topPanels =
                    hbox({
                        draculaPanel,
                        text(" "),
                        boardPanel,
                        text(" "),
                        sherlockPanel,
                    }) |
                    size(HEIGHT, EQUAL, 20);

                Element middlePanels =
                    hbox({renderHandPanel(handMenu),
                          renderActionPanel(
                              activeMenu == 0
                                  ? actionMenu
                              : activeMenu == 1
                                  ? fighterMenu
                              : activeMenu == 2
                                  ? destinationMenu
                              : activeMenu == 3
                                  ? targetMenu
                              : activeMenu == 4
                                  ? attackCardMenu
                              : activeMenu == 5
                                  ? defenseCardMenu
                              : activeMenu == 6
                                  ? schemeCardMenu
                                  : abilityMenu)}) |
                    size(HEIGHT, EQUAL, 9);

                Element bottomPanels =
                    hbox({
                        renderLogPanel(game.getActionLog()),

                        renderSelectionPanel(
                            actions[selectedAction], game.getActionsRemaining()),
                    }) |
                    size(HEIGHT, EQUAL, 9);

                return vbox({
                           header,
                           currentTurn,
                           topPanels,
                           middlePanels,
                           bottomPanels,
                       }) |
                       flex;
            });

    screen.Loop(renderer);

    if (openHelp)
    {
        runHelp();
        runGameDashboard();
    }
    else if (returnToMenu)
    {
        return;
    }
};