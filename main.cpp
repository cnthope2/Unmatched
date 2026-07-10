#include <iostream>
#include "game.hpp"
#include "sherlock.hpp"
#include "dracula.hpp"

using namespace std;

int main()
{
    Sherlock sherlock;
    Dracula dracula;

    Game game(&sherlock, &dracula);

    // کنار هم قرار می‌گیرند
    game.placeCharacter(&sherlock, 1);
    game.placeCharacter(&dracula, 2);

    // دراکولا Feint را برای حمله بازی می‌کند
    Card feintAttack(
        "Feint",
        CardType::Versatile,
        FighterType::Any,
        CombatTiming::BeforeCombat,
        CardEffect::Feint,
        1,
        2,
        2);

    // شرلوک Counter Punch را برای دفاع بازی می‌کند
    Card counterPunchDefense(
        "Counter Punch",
        CardType::Versatile,
        FighterType::Hero,
        CombatTiming::AfterCombat,
        CardEffect::CounterPunch,
        1,
        3,
        3);

    cout << "Dracula HP before: "
         << dracula.getHealth()
         << endl;

    cout << "Sherlock HP before: "
         << sherlock.getHealth()
         << endl;

    game.resolveCombat(
        &dracula,
        &sherlock,
        feintAttack,
        &counterPunchDefense);

    cout << "Dracula HP after: "
         << dracula.getHealth()
         << endl;

    cout << "Sherlock HP after: "
         << sherlock.getHealth()
         << endl;

    return 0;
}