#include "sisters.hpp"
Sister::Sister() : Character("Sister", 1, 2, AttackRange::Melee)
{
}

void Sister::revive()
{
    heal(1);
}