#include "Dracula.hpp"
using namespace std;

Dracula::Dracula() : Character("Dracula", 13, 2)
{
    sisters.push_back(Sister());
    sisters.push_back(Sister());
    sisters.push_back(Sister());
}

void Dracula::useAbility(Character &target)
{
    target.takeDamage(1);
}

int Dracula::getAliveSistersCount() const
{
    int count = 0;

    for (const auto &sister : sisters)
    {
        if (sister.isAlive())
        {
            count++;
        }
    }

    return count;
}

const vector<Sister> &Dracula::getSisters() const
{
    return sisters;
}

void Dracula::initializeDeck()
{
    deck.addCard(
        Card(
            "Feeding Frenzy",
            CardType::Attack,
            FighterType::Hero,
            CombatTiming::DuringCombat,
            CardEffect::FeedingFrenzy,
            3,
            2,
            0));

    deck.addCard(
        Card(
            "Feeding Frenzy",
            CardType::Attack,
            FighterType::Hero,
            CombatTiming::DuringCombat,
            CardEffect::FeedingFrenzy,
            3,
            2,
            0));
}

void Dracula::drawCard()
{
    if (!deck.isEmpty())
    {
        hand.addCard(deck.drawCard());
    }
}
int Dracula::getDeckSize() const
{
    return deck.size();
}
int Dracula::getHandSize() const
{
    return hand.size();
}