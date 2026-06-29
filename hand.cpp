#include "hand.hpp"
#include <stdexcept>

void Hand::addCard(const Card &card)
{
    cards.push_back(card);
}

void Hand::removeCard(int index)
{
    if (index < 0 || index >= cards.size())
    {
        throw std::out_of_range("Invalid card index");
    }

    cards.erase(cards.begin() + index);
}

int Hand::size() const
{
    return cards.size();
}

const std::vector<Card> &Hand::getCards() const
{
    return cards;
}