#ifndef SHERLOCK_HPP
#define SHERLOCK_HPP

#include "character.hpp"
#include "watson.hpp"
#include "deck.hpp"
#include "hand.hpp"

#include <vector>
using namespace std;

class Sherlock : public Character
{
private:
    Watson watson;

public:
    Sherlock();
    void useAbility(Character &target);
    const Watson &getWatson() const;
    Watson &getWatson();
    vector<Character *> getSidekicks() override;
    bool canBeDisabledByEffect(const Card &card) const override;
    void initializeDeck();
    void drawCard();
    int getDeckSize() const;
    int getHandSize() const;
    void drawStartingHand();
};

#endif
