#include "game.hpp"
#include <stdexcept>
#include <limits>
#include <iostream>
using namespace std;

Game::Game(Character *p1, Character *p2)
{
    player1 = p1;
    player2 = p2;
    currentPlayer = 1;
}
void Game::placeCharacter(Character *character, int spaceId)
{
    if (character == nullptr)
    {
        throw invalid_argument("Character pointer is null.");
    }

    if (spaceId < 1 || spaceId > board.getNumberOfSpaces())
    {
        throw out_of_range("Invalid space ID.");
    }

    Space *space = board.getSpace(spaceId);

    if (space->isOccupied())
    {
        throw runtime_error("Space is already occupied.");
    }

    character->setPosition(spaceId);
    space->setOccupant(character);
}

void Game::startGame()
{
    int age1, age2;

    cout << "Enter age of Player 1: ";
    cin >> age1;

    cout << "Enter age of Player 2: ";
    cin >> age2;

    bool player1IsYounger;

    if (age1 < age2)
    {
        player1IsYounger = true;
    }
    else
    {
        player1IsYounger = false;
    }

    initialPlacement(player1IsYounger);
}
void Game::placeSidekicks(Character *player)
{
    Space *heroSpace = board.getSpace(player->getPosition());

    const vector<Zone> &zones = heroSpace->getZones();

    vector<Character *> sidekicks = player->getSidekicks();

    for (Character *sidekick : sidekicks)
    {
        vector<Space *> availableSpaces;

        for (Zone zone : zones)
        {
            vector<Space *> spacesInZone = board.getSpacesInZone(zone);

            for (Space *space : spacesInZone)
            {
                if (!space->isOccupied())
                {
                    bool exists = false;

                    for (Space *s : availableSpaces)
                    {
                        if (s->getId() == space->getId())
                        {
                            exists = true;
                            break;
                        }
                    }

                    if (!exists)
                    {
                        availableSpaces.push_back(space);
                    }
                }
            }
        }

        cout << endl;

        cout << "Choose a space for " << sidekick->getName() << ":" << endl;

        for (Space *space : availableSpaces)
        {
            cout << space->getId() << " ";
        }

        cout << endl;

        int choice;

        while (true)
        {
            try
            {
                cin >> choice;
                if (cin.fail())
                {
                    cin.clear();
                    cin.ignore(1000, '\n');
                    throw invalid_argument("Please enter a number.");
                }

                bool valid = false;

                for (Space *space : availableSpaces)
                {
                    if (space->getId() == choice)
                    {
                        valid = true;
                        break;
                    }
                }

                cout << endl;
                if (!valid)
                {
                    throw invalid_argument("Invalid space.");
                }

                placeCharacter(sidekick, choice);
                break;
            }
            catch (const exception &e)
            {
                cout << e.what() << endl;
            }
        }
    }
}
void Game::playTurn()
{
}

void Game::initialPlacement(bool player1IsYounger)
{
    Character *youngerPlayer;
    Character *olderPlayer;

    int youngerPlayerNumber;

    if (player1IsYounger)
    {
        youngerPlayer = player1;
        olderPlayer = player2;
        youngerPlayerNumber = 1;
    }
    else
    {
        youngerPlayer = player2;
        olderPlayer = player1;
        youngerPlayerNumber = 2;
    }
    int choice;

    while (true)
    {
        try
        {

            cout << "Player " << youngerPlayerNumber << ", choose your starting space:" << endl;
            cout << "1. Space 1" << endl;
            cout << "2. Space 2" << endl;
            cout << "Choice: ";
            cin >> choice;

            if (cin.fail())
            {
                cin.clear();
                cin.ignore(1000, '\n');
                throw invalid_argument("Please enter a number.");
            }

            if (choice != 1 && choice != 2)
            {
                throw invalid_argument("Choice must be 1 or 2.");
            }

            break;
        }
        catch (const exception &e)
        {
            cout << e.what() << endl;
        }
    }
    if (choice == 1)
    {
        placeCharacter(youngerPlayer, 2);
        placeCharacter(olderPlayer, 23);
    }
    else
    {
        placeCharacter(youngerPlayer, 23);
        placeCharacter(olderPlayer, 2);
    }
    placeSidekicks(youngerPlayer);
    placeSidekicks(olderPlayer);
}

void Game::nextTurn()
{
    if (currentPlayer == 1)
    {
        currentPlayer = 2;
    }
    else
    {
        currentPlayer = 1;
    }
}

bool Game::isGameOver() const
{
    return !player1->isAlive() || !player2->isAlive();
}

Board &Game::getBoard()
{
    return board;
}

Character *Game::getCurrentPlayer() const
{
    if (currentPlayer == 1)
    {
        return player1;
    }

    return player2;
}

Character *Game::getOpponent() const
{
    if (currentPlayer == 1)
    {
        return player2;
    }

    return player1;
}
void Game::moveCharacter(Character *character, int destination)
{
    if (character == nullptr)
    {
        throw invalid_argument("Character pointer is null.");
    }

    if (destination < 1 || destination > board.getNumberOfSpaces())
    {
        throw out_of_range("Invalid destination.");
    }

    if (character->getPosition() != 0)
    {
        Space *current = board.getSpace(character->getPosition());
        current->setOccupant(nullptr);
    }

    Space *next = board.getSpace(destination);

    if (next->isOccupied())
    {
        throw runtime_error("Destination is occupied.");
    }

    next->setOccupant(character);
    character->setPosition(destination);
}