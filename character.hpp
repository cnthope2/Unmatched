#ifndef CHARACTER_HPP
#define CHARACTER_HPP
#include "deck.hpp"
#include "discardpile.hpp"
#include "hand.hpp"
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
    Deck deck;
    DiscardPile discardPile;
    Hand hand;

public:
    void addCardToHandForTest(const Card &card);









    
    Character(const string &name, int health, int movement);
    virtual ~Character() = default;
    virtual vector<Character *> getSidekicks();
    void takeDamage(int damage);
    void heal(int amount);
    bool isAlive() const;
    int getHealth() const;
    int getPosition() const;
    int getMovement() const;
    void setPosition(int newPosition);
    int getZoneId() const;
    void showHand() const;
    bool useBoost(int cardIndex);
    void setZoneId(int zone);
    string getName() const;
    Card getCardFromHand(int index) const;
    void drawCard();
    int getHandSize() const;
    void drawStartingHand();
    void discardExcessCards();
    void discardCard(int index);
    const Hand &getHand() const;
    virtual bool canBeDisabledByEffect(const Card &card) const;
};
#endif