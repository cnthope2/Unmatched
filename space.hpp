#ifndef SPACE_HPP
#define SPACE_HPP

#include <vector>
#include "character.hpp"
using namespace std;

class Space;

class Space
{
private:
    int id;
    vector<int> zones;
    bool secretPassage;
    Character *occupant;
    vector<Space *> adjacentSpaces;

public:
    Space(int id);
    int getId() const;
    void addAdjacent(Space *space);
    const vector<Space *> &getAdjacentSpaces() const;
    void addZone(int zone);
    const vector<int> &getZones() const;
    void setSecretPassage(bool value);
    bool hasSecretPassage() const;
    bool isOccupied() const;
    Character *getOccupant() const;
    void setOccupant(Character *fighter);
};

#endif