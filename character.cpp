#include "character.hpp"
#include <vector>
using namespace std;
#include <stdexcept>

Character::Character(const string &name, int health, int movement)
    : name(name),
      health(health),
      maxHealth(health),
      movement(movement),
      position(0),
      zoneId(0)

{
}
vector<Character*> Character::getSidekicks()
{
    return {};
}
void Character::takeDamage(int damage)
{
    if (damage < 0)
    {
        throw std::invalid_argument("Damage cannot be negative");
    }
    health -= damage;

    if (health < 0)
    {
        health = 0;
    }
}

void Character::heal(int amount)
{
    if (amount < 0)
    {
        throw std::invalid_argument("Heal amount cannot be negative");
    }

    health += amount;

    if (health > maxHealth)
    {
        health = maxHealth;
    }
}

bool Character::isAlive() const
{
    return health > 0;
}

int Character::getHealth() const
{
    return health;
}

int Character::getPosition() const
{
    return position;
}

void Character::setPosition(int newPosition)
{
    position = newPosition;
}
int Character::getZoneId() const
{
    return zoneId;
}

void Character::setZoneId(int zone)
{
    zoneId = zone;
}

string Character::getName() const
{
    return name;
}