#include "hand.hpp"

#include <stdexcept>

void Hand::addCard(const Card& card)
{
    cards.push_back(card);
}

void Hand::removeCard(int index)
{
    if (index < 0 || index >= static_cast<int>(cards.size()))
    {
        throw std::out_of_range("Invalid card index.");
    }

    cards.erase(cards.begin() + index);
}

const Card& Hand::getCard(int index) const
{
    if (index < 0 || index >= static_cast<int>(cards.size()))
    {
        throw std::out_of_range("Invalid card index.");
    }

    return cards[index];
}

bool Hand::isEmpty() const
{
    return cards.empty();
}

int Hand::size() const
{
    return static_cast<int>(cards.size());
}

const std::vector<Card>& Hand::getCards() const
{
    return cards;
}