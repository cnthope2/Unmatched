// #include "tui.hpp"

// #include <string>
// #include <vector>

// #include <ftxui/component/component.hpp>
// #include <ftxui/component/screen_interactive.hpp>
// #include <ftxui/dom/elements.hpp>

// using namespace ftxui;

// namespace
// {
// Element renderHeader()
// {
//     return vbox({
//                text("UNMATCHED") | bold | center,
//                text("Dracula vs Sherlock Holmes") | dim | center,
//            }) |
//            border;
// }

// Element renderPlayerPanel(const std::string& title)
// {
//     return vbox({
//                text(title) | bold | center,
//                separator(),
//                text("Player information"),
//                text("HP"),
//                text("Position"),
//                text("Deck"),
//                text("Hand"),
//                text("Discard"),
//            }) |
//            border |
//            size(WIDTH, EQUAL, 26) |
//            flex;
// }

// Element renderBoardPanel()
// {
//     return vbox({
//                text("BOARD") | bold | center,
//                separator(),
//                filler(),
//                text("Baskerville Manor") | dim | center,
//                text("Board will be rendered here") | center,
//                filler(),
//            }) |
//            border |
//            flex;
// }

// Element renderHandPanel()
// {
//     return vbox({
//                text("CURRENT HAND") | bold | center,
//                separator(),
//                text("Cards will be shown here"),
//            }) |
//            border |
//            flex;
// }

// Element renderActionPanel(Component actionMenu)
// {
//     return vbox({
//                text("ACTIONS") | bold | center,
//                separator(),
//                actionMenu->Render() | frame | flex,
//            }) |
//            border |
//            size(WIDTH, EQUAL, 30);
// }

// Element renderLogPanel()
// {
//     return vbox({
//                text("ACTION LOG") | bold,
//                separator(),
//                text("> Game log will appear here."),
//            }) |
//            border |
//            flex;
// }
// }

// void TUI::run()
// {
//     runMainMenu();
// }

// void TUI::runMainMenu()
// {
//     auto screen =
//         ScreenInteractive::TerminalOutput();

//     std::vector<std::string> entries = {
//         "Play",
//         "Help",
//         "Exit"
//     };

//     int selected = 0;

//     MenuOption option;

//     option.on_enter = [&]
//     {
//         if (selected == 0)
//         {
//             screen.ExitLoopClosure()();
//         }
//         else if (selected == 1)
//         {
//             screen.ExitLoopClosure()();
//         }
//         else if (selected == 2)
//         {
//             screen.ExitLoopClosure()();
//         }
//     };

//     Component menu =
//         Menu(&entries, &selected, option);

//     Component renderer =
//         Renderer(
//             menu,
//             [&]
//             {
//                 return vbox({
//                            text("UNMATCHED") |
//                                bold |
//                                center,

//                            text("Dracula vs Sherlock Holmes") |
//                                dim |
//                                center,

//                            separator(),

//                            menu->Render() |
//                                frame |
//                                size(WIDTH, EQUAL, 30),

//                            separator(),

//                            text("Use Arrow Keys and Enter") |
//                                dim |
//                                center
//                        }) |
//                        border |
//                        center;
//             });

//     screen.Loop(renderer);

//     if (selected == 0)
//     {
//         runGameDashboard();
//     }
//     else if (selected == 1)
//     {
//         runHelp();
//         runMainMenu();
//     }
// }

// void TUI::runHelp()
// {
//     auto screen =
//         ScreenInteractive::TerminalOutput();

//     Component backButton =
//         Button(
//             "Back",
//             screen.ExitLoopClosure());

//     Component renderer =
//         Renderer(
//             backButton,
//             [&]
//             {
//                 return vbox({
//                            text("HELP") |
//                                bold |
//                                center,

//                            separator(),

//                            text("Arrow Keys: Navigate"),
//                            text("Enter: Select"),
//                            text("Maneuver: Draw and move"),
//                            text("Attack: Choose fighter and target"),
//                            text("Scheme: Play a Scheme card"),
//                            text("Ability: Use Dracula's ability"),

//                            separator(),

//                            backButton->Render() |
//                                center
//                        }) |
//                        border |
//                        size(WIDTH, EQUAL, 50) |
//                        center;
//             });

//     screen.Loop(renderer);
// }

// void TUI::runGameDashboard()
// {
//     auto screen =
//         ScreenInteractive::Fullscreen();

//     std::vector<std::string> actions = {
//         "Maneuver",
//         "Attack",
//         "Scheme",
//         "Use Ability",
//         "Help",
//         "Back to Main Menu",
//     };

//     int selectedAction = 0;

//     MenuOption option;

//     option.on_enter = [&]
//     {
//         if (selectedAction == 4)
//         {
//             screen.ExitLoopClosure()();
//         }
//         else if (selectedAction == 5)
//         {
//             screen.ExitLoopClosure()();
//         }
//     };

//     Component actionMenu =
//         Menu(
//             &actions,
//             &selectedAction,
//             option);

//     Component renderer =
//         Renderer(
//             actionMenu,
//             [&]
//             {
//                 Element header =
//                     renderHeader();

//                 Element currentTurn =
//                     text("Current Turn: Dracula") |
//                     bold |
//                     color(Color::Red) |
//                     center;

//                 Element topPanels =
//                     hbox({
//                         renderPlayerPanel("DRACULA"),
//                         renderBoardPanel(),
//                         renderPlayerPanel("SHERLOCK HOLMES"),
//                     }) |
//                     flex;

//                 Element middlePanels =
//                     hbox({
//                         renderHandPanel(),
//                         renderActionPanel(actionMenu),
//                     }) |
//                     flex;

//                 Element logPanel =
//                     renderLogPanel();

//                 return vbox({
//                            header,
//                            currentTurn,
//                            topPanels,
//                            middlePanels,
//                            logPanel,
//                        }) |
//                        flex;
//             });

//     screen.Loop(renderer);

//     if (selectedAction == 4)
//     {
//         runHelp();
//         runGameDashboard();
//     }
//     else if (selectedAction == 5)
//     {
//         runMainMenu();
//     }
// }