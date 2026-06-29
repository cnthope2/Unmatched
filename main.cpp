#include "deck.hpp"
#include "dracula.hpp"
#include <iostream>

int main()
{
    Dracula d;

    d.initializeDeck();

    std::cout << d.getDeckSize() << std::endl;

    d.drawCard();

    std::cout << d.getDeckSize() << std::endl;

    std::cout << d.getHandSize() << std::endl;
    return 0;
}