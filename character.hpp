#ifndef CHARACTER_HPP
#define CHARACTER_HPP
#include <string>
#include <vector>
using namespace std;

class Character
{
protected:
    string name;
    int health;
    int movement;
    int position;
    int zoneId;
    int maxHealth;

public:
    Character(const string &name, int health, int movement);
    virtual ~Character() = default;
    virtual vector<Character*> getSidekicks();
    void takeDamage(int damage);
    void heal(int amount);
    bool isAlive() const;
    int getHealth() const;
    int getPosition() const;
    void setPosition(int newPosition);
    int getZoneId() const;
    void setZoneId(int zone);
    string getName() const;
};
#endif