#ifndef DISCARD_PILE_HPP
#define DISCARD_PILE_HPP

#include <vector>
#include "card.hpp"

class DiscardPile
{
private:
    std::vector<Card> cards;

public:
    void addCard(const Card& card);
    int size() const;
    bool isEmpty() const;
    const std::vector<Card>& getCards() const;
};

#endif