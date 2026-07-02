#include "board.hpp"

Board::Board()
{
    initializeSpaces();
    initializeZones();
    initializeAdjacency();
    initializeSecretPassages();
}

void Board::initializeSpaces()
{
    for (int i = 1; i <= 32; i++)
    {
        spaces.push_back(Space(i));
    }
}

void Board::initializeZones()
{
}

void Board::initializeAdjacency()
{
}

void Board::initializeSecretPassages()
{
}
Space* Board::getSpace(int id)
{
    if (id < 1 || id > spaces.size())
    {
        return nullptr;
    }

    return &spaces[id - 1];
}

int Board::getNumberOfSpaces() const
{
    return spaces.size();
}