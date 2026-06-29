#ifndef HAND_HPP
#define HAND_HPP

#include <vector>
#include "card.hpp"
using namespace std;

class Hand
{
private:
   vector<Card> cards;

public:
    void addCard(const Card& card);

    void removeCard(int index);

    int size() const;

    const vector<Card>& getCards() const;
};

#endif