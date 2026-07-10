#include "card.hpp"
using namespace std;

Card::Card(
    const string &name,
    CardType type,
    FighterType fighterType,
    CombatTiming timing,
    CardEffect effect,
    int boost,
    int attackValue,
    int defenseValue)
    : name(name),
      type(type),
      fighterType(fighterType),
      timing(timing),
      effect(effect),
      boost(boost),
      attackValue(attackValue),
      defenseValue(defenseValue)
{
}

string Card::getName() const
{
    return name;
}

CardType Card::getType() const
{
    return type;
}


CombatTiming Card::getTiming() const
{
    return timing;
}

CardEffect Card::getEffect() const
{
    return effect;
}
FighterType Card::getFighterType() const
{
    return fighterType;
}
int Card::getBoost() const
{
    return boost;
}

int Card::getAttackValue() const
{
    return attackValue;
}

int Card::getDefenseValue() const
{
    return defenseValue;
}