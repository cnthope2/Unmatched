#include "board.hpp"
#include <stdexcept>
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
    spaces[0].addZone(Zone::Purple);
    spaces[0].addZone(Zone::Green);

    spaces[1].addZone(Zone::Purple);

    spaces[2].addZone(Zone::Purple);

    spaces[3].addZone(Zone::Purple);

    spaces[4].addZone(Zone::Green);

    spaces[5].addZone(Zone::Green);

    spaces[6].addZone(Zone::Green);
    spaces[6].addZone(Zone::Gray);

    spaces[7].addZone(Zone::Gray);

    spaces[8].addZone(Zone::Gray);

    spaces[9].addZone(Zone::Purple);
    spaces[9].addZone(Zone::Gray);

    spaces[10].addZone(Zone::Gray);

    spaces[11].addZone(Zone::Gray);

    spaces[12].addZone(Zone::Gray);

    spaces[13].addZone(Zone::Gray);

    spaces[14].addZone(Zone::Gray);

    spaces[15].addZone(Zone::Blue);
    spaces[15].addZone(Zone::Gray);

    spaces[16].addZone(Zone::Blue);
    spaces[16].addZone(Zone::Green);

    spaces[17].addZone(Zone::Blue);

    spaces[18].addZone(Zone::Blue);

    spaces[19].addZone(Zone::Blue);
    spaces[19].addZone(Zone::LightBlue);

    spaces[20].addZone(Zone::LightBlue);

    spaces[21].addZone(Zone::LightBlue);

    spaces[22].addZone(Zone::Brown);
    spaces[22].addZone(Zone::LightBlue);

    spaces[23].addZone(Zone::LightBlue);

    spaces[24].addZone(Zone::LightBlue);

    spaces[25].addZone(Zone::Brown);

    spaces[26].addZone(Zone::Brown);

    spaces[27].addZone(Zone::Brown);

    spaces[28].addZone(Zone::Cream);
    spaces[28].addZone(Zone::Brown);
    spaces[28].addZone(Zone::Green);

    spaces[29].addZone(Zone::Cream);

    spaces[30].addZone(Zone::Cream);

    spaces[31].addZone(Zone::Cream);
}

void Board::initializeAdjacency()
{
    spaces[0].addAdjacent(&spaces[1]);
    spaces[0].addAdjacent(&spaces[2]);
    spaces[0].addAdjacent(&spaces[4]);
    spaces[0].addAdjacent(&spaces[7]);
    spaces[0].addAdjacent(&spaces[8]);
    spaces[0].addAdjacent(&spaces[28]);

    spaces[1].addAdjacent(&spaces[0]);
    spaces[1].addAdjacent(&spaces[3]);

    spaces[2].addAdjacent(&spaces[0]);
    spaces[2].addAdjacent(&spaces[3]);
    spaces[2].addAdjacent(&spaces[30]);

    spaces[3].addAdjacent(&spaces[1]);
    spaces[3].addAdjacent(&spaces[2]);
    spaces[3].addAdjacent(&spaces[9]);

    spaces[4].addAdjacent(&spaces[0]);
    spaces[4].addAdjacent(&spaces[5]);

    spaces[5].addAdjacent(&spaces[4]);
    spaces[5].addAdjacent(&spaces[6]);
    spaces[5].addAdjacent(&spaces[16]);

    spaces[6].addAdjacent(&spaces[5]);
    spaces[6].addAdjacent(&spaces[7]);
    spaces[6].addAdjacent(&spaces[11]);
    spaces[6].addAdjacent(&spaces[12]);

    spaces[7].addAdjacent(&spaces[0]);
    spaces[7].addAdjacent(&spaces[6]);
    spaces[7].addAdjacent(&spaces[8]);
    spaces[7].addAdjacent(&spaces[11]);

    spaces[8].addAdjacent(&spaces[0]);
    spaces[8].addAdjacent(&spaces[7]);
    spaces[8].addAdjacent(&spaces[9]);
    spaces[8].addAdjacent(&spaces[10]);

    spaces[9].addAdjacent(&spaces[3]);
    spaces[9].addAdjacent(&spaces[8]);
    spaces[9].addAdjacent(&spaces[10]);

    spaces[10].addAdjacent(&spaces[8]);
    spaces[10].addAdjacent(&spaces[9]);

    spaces[11].addAdjacent(&spaces[6]);
    spaces[11].addAdjacent(&spaces[7]);
    spaces[11].addAdjacent(&spaces[12]);

    spaces[12].addAdjacent(&spaces[6]);
    spaces[12].addAdjacent(&spaces[11]);
    spaces[12].addAdjacent(&spaces[13]);

    spaces[13].addAdjacent(&spaces[12]);
    spaces[13].addAdjacent(&spaces[14]);

    spaces[14].addAdjacent(&spaces[13]);
    spaces[14].addAdjacent(&spaces[15]);

    spaces[15].addAdjacent(&spaces[14]);
    spaces[15].addAdjacent(&spaces[16]);
    spaces[15].addAdjacent(&spaces[17]);

    spaces[16].addAdjacent(&spaces[5]);
    spaces[16].addAdjacent(&spaces[15]);

    spaces[17].addAdjacent(&spaces[15]);
    spaces[17].addAdjacent(&spaces[18]);
    spaces[17].addAdjacent(&spaces[20]);

    spaces[18].addAdjacent(&spaces[17]);
    spaces[18].addAdjacent(&spaces[19]);

    spaces[19].addAdjacent(&spaces[18]);
    spaces[19].addAdjacent(&spaces[21]);
    spaces[19].addAdjacent(&spaces[22]);

    spaces[20].addAdjacent(&spaces[17]);
    spaces[20].addAdjacent(&spaces[21]);
    spaces[20].addAdjacent(&spaces[23]);

    spaces[21].addAdjacent(&spaces[19]);
    spaces[21].addAdjacent(&spaces[20]);

    spaces[22].addAdjacent(&spaces[24]);
    spaces[22].addAdjacent(&spaces[25]);
    spaces[22].addAdjacent(&spaces[19]);

    spaces[23].addAdjacent(&spaces[20]);
    spaces[23].addAdjacent(&spaces[24]);

    spaces[24].addAdjacent(&spaces[22]);
    spaces[24].addAdjacent(&spaces[23]);

    spaces[25].addAdjacent(&spaces[22]);
    spaces[25].addAdjacent(&spaces[26]);
    spaces[25].addAdjacent(&spaces[27]);

    spaces[26].addAdjacent(&spaces[25]);
    spaces[26].addAdjacent(&spaces[27]);
    spaces[26].addAdjacent(&spaces[28]);

    spaces[27].addAdjacent(&spaces[25]);
    spaces[27].addAdjacent(&spaces[26]);
    spaces[27].addAdjacent(&spaces[28]);

    spaces[28].addAdjacent(&spaces[0]);
    spaces[28].addAdjacent(&spaces[26]);
    spaces[28].addAdjacent(&spaces[27]);
    spaces[28].addAdjacent(&spaces[29]);
    spaces[28].addAdjacent(&spaces[30]);

    spaces[29].addAdjacent(&spaces[28]);
    spaces[29].addAdjacent(&spaces[30]);
    spaces[29].addAdjacent(&spaces[31]);

    spaces[30].addAdjacent(&spaces[2]);
    spaces[30].addAdjacent(&spaces[28]);
    spaces[30].addAdjacent(&spaces[29]);

    spaces[31].addAdjacent(&spaces[29]);
}

void Board::initializeSecretPassages()
{
    spaces[4].setSecretPassage(true);
    spaces[14].setSecretPassage(true);
    spaces[23].setSecretPassage(true);
    spaces[31].setSecretPassage(true);
}

Space *Board::getSpace(int id)
{
    if (id < 1 || id > spaces.size())
        throw out_of_range("Invalid space ID.");

    return &spaces[id - 1];
}

int Board::getNumberOfSpaces() const
{
    return spaces.size();
}