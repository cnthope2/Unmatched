#ifndef HAND_HPP
#define HAND_HPP

#include <vector>
#include "card.hpp"

class Hand
{
private:
    std::vector<Card> cards;

public:
    void addCard(const Card& card);
    void removeCard(int index);
    const Card& getCard(int index) const;
    bool isEmpty() const;
    int size() const;
    const std::vector<Card>& getCards() const;
};

#endif