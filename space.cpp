#include "space.hpp"
using namespace std;

Space::Space(int id)
{
    this->id = id;
    secretPassage = false;
    occupant = nullptr;
}

int Space::getId() const
{
    return id;
}

void Space::addAdjacent(Space* space)
{
    adjacentSpaces.push_back(space);
}

const vector<Space*>& Space::getAdjacentSpaces() const
{
    return adjacentSpaces;
}

void Space::addZone(Zone zone)
{
    zones.push_back(zone);
}

const vector<Zone>& Space::getZones() const
{
    return zones;
}

void Space::setSecretPassage(bool value)
{
    secretPassage = value;
}

bool Space::hasSecretPassage() const
{
    return secretPassage;
}

bool Space::isInZone(Zone zone) const
{
    for (Zone z : zones)
    {
        if (z == zone)
        {
            return true;
        }
    }

    return false;
}
bool Space::isOccupied() const
{
    return occupant != nullptr;
}

Character* Space::getOccupant() const
{
    return occupant;
}

void Space::setOccupant(Character* fighter)
{
    occupant = fighter;
}