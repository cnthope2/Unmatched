#include "sisters.hpp"

Sister::Sister() : Character("Sister", 1, 2)
{
}
void Sister::revive()
{
    heal(1);
}