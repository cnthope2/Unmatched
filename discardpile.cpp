#include "discardpile.hpp"

void DiscardPile::addCard(const Card& card)
{
    cards.push_back(card);
}

int DiscardPile::size() const
{
    return cards.size();
}

bool DiscardPile::isEmpty() const
{
    return cards.empty();
}

const std::vector<Card>& DiscardPile::getCards() const
{
    return cards;
}