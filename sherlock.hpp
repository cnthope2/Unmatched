#ifndef SHERLOCK_HPP
#define SHERLOCK_HPP

#include "character.hpp"
#include "watson.hpp"
#include "deck.hpp"
#include "hand.hpp"

using namespace std;

class Sherlock : public Character
{
private:
    Watson watson;
    Deck deck;
    Hand hand;

public:
    Sherlock();
    void useAbility(Character &target);
    const Watson &getWatson() const;
    void initializeDeck();
    void drawCard();
    int getDeckSize() const;
    int getHandSize() const;
    void drawStartingHand();
};

#endif

