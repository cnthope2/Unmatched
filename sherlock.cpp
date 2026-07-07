#include "sherlock.hpp"

Sherlock::Sherlock() : Character("Sherlock Holmes", 16, 2)
{
    initializeDeck();
    drawStartingHand();
}
void Sherlock::useAbility(Character &target)
{
   
}
void Sherlock::initializeDeck()
{

    deck.addCopies(Card(
                       "Aid Dr. Watson",
                       CardType::Scheme,
                       FighterType::Sidekick,
                       CombatTiming::None,
                       CardEffect::AidDrWatson,
                       2, 0, 0),
                   2);

    deck.addCopies(Card(
                       "Confirm Suspicion",
                       CardType::Scheme,
                       FighterType::Hero,
                       CombatTiming::None,
                       CardEffect::ConfirmSuspicion,
                       1, 0, 0),
                   3);

    deck.addCopies(Card(
                       "Counter Punch",
                       CardType::Versatile,
                       FighterType::Hero,
                       CombatTiming::AfterCombat,
                       CardEffect::CounterPunch,
                       1, 3, 3),
                   3);

    deck.addCopies(Card(
                       "Deduce Strategy",
                       CardType::Versatile,
                       FighterType::Hero,
                       CombatTiming::DuringCombat,
                       CardEffect::DeduceStrategy,
                       1, 3, 3),
                   3);

    deck.addCopies(Card(
                       "Education Never Ends",
                       CardType::Versatile,
                       FighterType::Any,
                       CombatTiming::AfterCombat,
                       CardEffect::EducationNeverEnds,
                       1, 3, 3),
                   2);

    deck.addCopies(Card(
                       "Elementary",
                       CardType::Defense,
                       FighterType::Hero,
                       CombatTiming::DuringCombat,
                       CardEffect::Elementary,
                       3, 0, 3),
                   2);

    deck.addCopies(Card(
                       "Eliminate the Impossible",
                       CardType::Scheme,
                       FighterType::Hero,
                       CombatTiming::None,
                       CardEffect::EliminateTheImpossible,
                       2, 0, 0),
                   2);

    deck.addCopies(Card(
                       "Feint",
                       CardType::Versatile,
                       FighterType::Any,
                       CombatTiming::BeforeCombat,
                       CardEffect::Feint,
                       1, 2, 2),
                   3);

    deck.addCopies(Card(
                       "Fixed Point in a Changing Age",
                       CardType::Versatile,
                       FighterType::Sidekick,
                       CombatTiming::AfterCombat,
                       CardEffect::FixedPointInAChangingAge,
                       1, 3, 3),
                   2);

    deck.addCopies(Card(
                       "Master of Disguise",
                       CardType::Scheme,
                       FighterType::Hero,
                       CombatTiming::None,
                       CardEffect::MasterOfDisguise,
                       2, 0, 0),
                   2);

    deck.addCopies(Card(
                       "Service Revolver",
                       CardType::Attack,
                       FighterType::Sidekick,
                       CombatTiming::None,
                       CardEffect::ServiceRevolver,
                       3, 5, 0),
                   2);

    deck.addCopies(Card(
                       "Study Methods",
                       CardType::Versatile,
                       FighterType::Any,
                       CombatTiming::AfterCombat,
                       CardEffect::StudyMethods,
                       2, 3, 3),
                   2);

    deck.addCopies(Card(
                       "The Game Is Afoot",
                       CardType::Attack,
                       FighterType::Hero,
                       CombatTiming::AfterCombat,
                       CardEffect::TheGameIsAfoot,
                       2, 5, 0),
                   2);
}


const Watson& Sherlock::getWatson() const
{
    return watson;
}


void Sherlock::drawCard()
{
    if (!deck.isEmpty())
    {
        hand.addCard(deck.drawCard());
    }
}

int Sherlock::getDeckSize() const
{
    return deck.size();
}

int Sherlock::getHandSize() const
{
    return hand.size();
}

void Sherlock::drawStartingHand()
{
    for (int i = 0; i < 5; i++)
    {
        drawCard();
    }
}