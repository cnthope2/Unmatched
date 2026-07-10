#ifndef BOARD_HPP
#define BOARD_HPP

#include <vector>
#include "space.hpp"

using namespace std;

class Board
{
private:
    vector<Space> spaces;
    void initializeSpaces();
    void initializeZones();
    void initializeAdjacency();
    void initializeSecretPassages();

public:
    Board();
    vector<Space *> getSpacesInZone(Zone zone);
    vector<Space *> getMovementNeighbors(Space *space);
    Space *getSpace(int id);
    const Space *getSpace(int id) const;
    int getNumberOfSpaces() const;
};

#endif