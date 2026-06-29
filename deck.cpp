#include "deck.hpp"
#include <stdexcept>

void Deck::addCard(const Card& card)
{
    cards.push_back(card);
}


Card Deck::drawCard()
{
    if (cards.empty())
    {
        throw std::runtime_error("Deck is empty");
    }

    Card card = cards.back();
    cards.pop_back();
    return card;
}

bool Deck::isEmpty() const
{
    return cards.empty();
}

int Deck::size() const
{
    return cards.size();
}