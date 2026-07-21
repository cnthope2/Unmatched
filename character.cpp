#include "character.hpp"
#include "discardpile.hpp"
#include <vector>
#include <iostream>
using namespace std;
#include <stdexcept>
Character::Character(
    const string &name,
    int health,
    int movement,
    AttackRange attackRange)
    : name(name),
      health(health),
      maxHealth(health),
      movement(movement),
      position(0),
      zoneId(0),
      attackRange(attackRange)
{
}

vector<Character *> Character::getSidekicks()
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
AttackRange Character::getAttackRange() const
{
    return attackRange;
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
int Character::getMaxHealth() const
{
    return maxHealth;

}
int Character::getHealth() const
{
    return health;
}

int Character::getPosition() const
{
    return position;
}
int Character::getMovement() const
{
    return movement;
}
const Deck &Character::getDeck() const
{
    return deck;
}

const DiscardPile &Character::getDiscardPile() const
{
    return discardPile;
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
void Character::drawCard()
{

    Card c = deck.drawCard();

    hand.addCard(c);
}
void Character::showHand() const
{
    const vector<Card> &cards = hand.getCards();

    cout << getName() << "'s Hand:" << endl;

    for (const Card &card : cards)
    {
        cout << "- " << card.getName() << endl;
    }

    cout << endl;
}

bool Character::useBoost(int cardIndex)
{
    if (cardIndex < 0 || cardIndex >= hand.size())
    {
        return false;
    }

    Card card = hand.getCard(cardIndex);

    if (card.getBoost() <= 0)
    {
        return false;
    }

    movement += card.getBoost(); 
    discardPile.addCard(card);     
    hand.removeCard(cardIndex);  

    return true;
}

void Character::drawStartingHand()
{
    for (int i = 0; i < 5; i++)
    {
        drawCard();
    }
}
void Character::discardCard(int index)
{
    if (index < 0 || index >= hand.size())
    {
        throw out_of_range("Invalid card index.");
    }

    Card card = hand.getCard(index);

    discardPile.addCard(card);

    hand.removeCard(index);
}

const Hand &Character::getHand() const
{
    return hand;
}
int Character::getHandSize() const
{
    return hand.size();
}
Card Character::getCardFromHand(int index) const
{
    if (index < 0 || index >= hand.size())
    {
        throw out_of_range("Invalid card index.");
    }

    return hand.getCard(index);
}
void Character::discardExcessCards()
{
    while (hand.size() > 7)
    {
        discardCard(hand.size() - 1);
    }
}
bool Character::canBeDisabledByEffect(const Card &card) const
{
    return true;
}