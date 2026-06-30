#ifndef CARD_HPP
#define CARD_HPP

#include <string>
using namespace std;

enum class CardType
{
    Attack,
    Defense,
    Versatile,
    Scheme
};

enum class FighterType
{
    Hero,
    Sidekick,
    Any
};

enum class CombatTiming
{
    None,
    Immediately,
    DuringCombat,
    BeforeCombat,
    AfterCombat
};

enum class CardEffect
{
    None,
    FeedingFrenzy,
    MistForm,
    Ambush,
    BaptismOfBlood,
    Beastform,
    Dash,
    Exploit,
    LookIntoMyEyes,
    PreyUpon,
    RaveningSeduction,
    ThirstForSustenance,
    Feint
};

class Card
{
private:
    string name;

    CardType type;
    FighterType fighterType;
    CombatTiming timing;
    CardEffect effect;

    int boost;

    int attackValue;
    int defenseValue;

public:
    Card(
        const string &name,
        CardType type,
        FighterType fighterType,
        CombatTiming timing,
        CardEffect effect,
        int boost,
        int attackValue,
        int defenseValue);
    string getName() const;
    CardType getType() const;
    FighterType getFighterType() const;
    CombatTiming getTiming() const;
    CardEffect getEffect() const;
    int getBoost() const;
    int getAttackValue() const;
    int getDefenseValue() const;
};

#endif