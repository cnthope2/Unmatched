#include <iostream>
#include "tui.hpp"
#include "character.hpp"
#include "game.hpp"
#include "sherlock.hpp"
#include "dracula.hpp"

using namespace std;

int main()
{
    Dracula dracula;
    Sherlock sherlock;
    int age1;
    int age2;

    cout << "Enter age of Player 1: ";
    cin >> age1;

    cout << "Enter age of Player 2: ";
    cin >> age2;

    int youngerPlayer;

    if (age1 < age2)
    {
        youngerPlayer = 1;
    }
    else if (age2 < age1)
    {
        youngerPlayer = 2;
    }
    else
    {
        youngerPlayer = 1;
        cout << "Ages are equal. Player 1 chooses first.\n";
    }

    cout << "\nPlayer " << youngerPlayer << " is younger and chooses a character first.\n";

    cout << "1. Sherlock Holmes\n";
    cout << "2. Dracula\n";
    cout << "Choice: ";

    int choice;

    while (true)
    {
        cin >> choice;

        if (!cin.fail() &&
            (choice == 1 || choice == 2))
        {
            break;
        }

        cin.clear();
        cin.ignore(1000, '\n');

        cout << "Invalid choice. Enter 1 or 2: ";
    }

    Character *player1 = nullptr;
    Character *player2 = nullptr;

    if (youngerPlayer == 1)
    {
        if (choice == 1)
        {
            player1 = new Sherlock();
            player2 = new Dracula();
        }
        else
        {
            player1 = new Dracula();
            player2 = new Sherlock();
        }
    }
    else
    {
        if (choice == 1)
        {
            player2 = new Sherlock();
            player1 = new Dracula();
        }
        else
        {
            player2 = new Dracula();
            player1 = new Sherlock();
        }
    }

    cout << "\nPlayer 1: " << player1->getName() << endl;

    cout << "Player 2: " << player2->getName() << endl;
    Game game(player1, player2);

    game.setupGame(youngerPlayer);

    TUI tui(game);
    tui.run();

    delete player1;
    delete player2;

    return 0;
}