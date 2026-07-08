#ifndef SPACE_HPP
#define SPACE_HPP

#include <vector>
#include "character.hpp"
using namespace std;

enum class Zone
{
    Purple,
    Green,
    Gray,
    Blue,
    LightBlue,
    Brown,
    Cream
};


class Space
{
private:
    int id;
    vector<Zone> zones;
    bool secretPassage;
    Character *occupant;
    vector<Space *> adjacentSpaces;

public:
    Space(int id);
    int getId() const;
    void addAdjacent(Space *space);
    const vector<Space *> &getAdjacentSpaces() const;
    void addZone(Zone zone);
    const vector<Zone> &getZones() const;
    void setSecretPassage(bool value);
    bool hasSecretPassage() const;
    bool isInZone(Zone zone) const;
    bool isOccupied() const;
    Character *getOccupant() const;
    void setOccupant(Character *fighter);
};

#endif