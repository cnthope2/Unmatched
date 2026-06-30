#include "deck.hpp"
#include "dracula.hpp"
#include <iostream>

int main()
{
    Dracula d;

    cout << d.getDeckSize() << endl;
    cout << d.getHandSize() << endl;

    return 0;
}