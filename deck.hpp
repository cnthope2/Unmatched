#ifndef DECK_HPP
#define DECK_HPP

#include <vector>
#include "card.hpp"

class Deck
{
private:
    std::vector<Card> cards;

public:
    void addCard(const Card &card);
    void addCopies(const Card &card, int count);
    Card drawCard();
    bool isEmpty() const;
    int size() const;
};

#endif