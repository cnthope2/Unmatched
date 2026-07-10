#include "Dracula.hpp"
#include<iostream>
using namespace std;

Dracula::Dracula() : Character("Dracula", 13, 2)
{
    sisters.push_back(Sister());
    sisters.push_back(Sister());
    sisters.push_back(Sister());
    initializeDeck();
   drawStartingHand();
}

void Dracula::useAbility(Character &target)
{
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
vector<Sister> &Dracula::getSisters()
{
    return sisters;
}
vector<Character*> Dracula::getSidekicks()
{
    vector<Character*> result;

    for (Sister& sister : sisters)
    {
        result.push_back(&sister);
    }

    return result;
}
void Dracula::initializeDeck()
{
    deck.addCopies(Card(
                       "Feeding Frenzy",
                       CardType::Attack,
                       FighterType::Hero,
                       CombatTiming::DuringCombat,
                       CardEffect::FeedingFrenzy,
                       3, 2, 0),
                   2);

    deck.addCopies(Card(
                       "Mist Form",
                       CardType::Scheme,
                       FighterType::Hero,
                       CombatTiming::None,
                       CardEffect::MistForm,
                       2, 0, 0),
                   2);

    deck.addCopies(Card(
                       "Ambush",
                       CardType::Attack,
                       FighterType::Any,
                       CombatTiming::DuringCombat,
                       CardEffect::Ambush,
                       3, 2, 0),
                   2);

    deck.addCopies(Card(
                       "Baptism of Blood",
                       CardType::Scheme,
                       FighterType::Hero,
                       CombatTiming::None,
                       CardEffect::BaptismOfBlood,
                       2, 0, 0),
                   2);

    deck.addCopies(Card(
                       "Beastform",
                       CardType::Attack,
                       FighterType::Hero,
                       CombatTiming::DuringCombat,
                       CardEffect::Beastform,
                       4, 6, 0),
                   2);

    deck.addCopies(Card(
                       "Dash",
                       CardType::Versatile,
                       FighterType::Any,
                       CombatTiming::AfterCombat,
                       CardEffect::Dash,
                       1, 3, 3),
                   3);

    deck.addCopies(Card(
                       "Exploit",
                       CardType::Versatile,
                       FighterType::Any,
                       CombatTiming::AfterCombat,
                       CardEffect::Exploit,
                       1, 4, 4),
                   3);

    deck.addCopies(Card(
                       "Look Into My Eyes",
                       CardType::Defense,
                       FighterType::Hero,
                       CombatTiming::DuringCombat,
                       CardEffect::LookIntoMyEyes,
                       2, 0, 1),
                   3);

    deck.addCopies(Card(
                       "Prey Upon",
                       CardType::Scheme,
                       FighterType::Hero,
                       CombatTiming::None,
                       CardEffect::PreyUpon,
                       4, 0, 0),
                   2);

    deck.addCopies(Card(
                       "Ravening Seduction",
                       CardType::Scheme,
                       FighterType::Sidekick,
                       CombatTiming::None,
                       CardEffect::RaveningSeduction,
                       2, 0, 0),
                   3);

    deck.addCopies(Card(
                       "Thirst for Sustenance",
                       CardType::Attack,
                       FighterType::Sidekick,
                       CombatTiming::AfterCombat,
                       CardEffect::ThirstForSustenance,
                       3, 3, 0),
                   3);

    deck.addCopies(Card(
                       "Feint",
                       CardType::Versatile,
                       FighterType::Any,
                       CombatTiming::BeforeCombat,
                       CardEffect::Feint,
                       2, 2, 2),
                   3);
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
void Dracula::drawStartingHand()
{
    for (int i = 0; i < 5; i++)
    {
        drawCard();
    }
}