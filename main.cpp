#include <iostream>
#include "game.hpp"
#include "sherlock.hpp"
#include "dracula.hpp"

using namespace std;

int main()
{
    Sherlock sherlock;
    Dracula dracula;

    Game game(&sherlock, &dracula);

    game.startGame();
    Board &board = game.getBoard();

    cout << "\n===== Board Status =====\n";

    for (int i = 1; i <= board.getNumberOfSpaces(); i++)
    {
        Space *space = board.getSpace(i);

        if (space->isOccupied())
        {
            cout << "Space "
                 << i
                 << " : "
                 << space->getOccupant()->getName()
                 << endl;
        }
    }

    return 0;
}