#ifndef DRACULA_HPP
#define DRACULA_HPP
#include "character.hpp"
#include "sisters.hpp"
#include "deck.hpp"
#include "hand.hpp"
using namespace std;

#include <vector>

class Dracula : public Character
{
private:
    vector<Sister> sisters;
    

public:
    Dracula();
    void useAbility(Character &target);
    int getAliveSistersCount() const;
    const vector<Sister> &getSisters() const;
    vector<Sister> &getSisters();
    vector<Character*> getSidekicks() override;
    void initializeDeck();
    void drawCard();
    int getDeckSize() const;
    int getHandSize() const;
    void drawStartingHand();
};
#endif