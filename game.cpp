#include "game.hpp"
#include "board.hpp"
#include <stdexcept>
#include <queue>
#include <limits>
#include <iostream>
using namespace std;

Game::Game(Character *p1, Character *p2)
{
    player1 = p1;
    player2 = p2;
    currentPlayer = 1;
    actionsRemaining = 2;
}
void Game::placeCharacter(Character *character, int spaceId)
{
    if (character == nullptr)
    {
        throw invalid_argument("Character pointer is null.");
    }

    if (spaceId < 1 || spaceId > board.getNumberOfSpaces())
    {
        throw out_of_range("Invalid space ID.");
    }

    Space *space = board.getSpace(spaceId);

    if (space->isOccupied())
    {
        throw runtime_error("Space is already occupied.");
    }

    character->setPosition(spaceId);
    space->setOccupant(character);
}

void Game::startGame(bool player1IsYounger)
{
    initialPlacement(player1IsYounger);

    if (player1IsYounger)
    {
        currentPlayer = 1;
    }
    else
    {
        currentPlayer = 2;
    }

    while (!isGameOver())
    {
        Character *player =
            getCurrentPlayer();

        playTurn(player);

        if (!isGameOver())
        {
            nextTurn();
        }
    }

    removeDefeatedSidekicks();

    cout << "\n..... GAME OVER ......\n";

    if (!player1->isAlive() && !player2->isAlive())
    {
        cout << "Both heroes were defeated.\n";
    }
    else if (!player1->isAlive())
    {
        cout << player2->getName() << " wins!\n";
    }
    else
    {
        cout << player1->getName() << " wins!\n";
    }
}

void Game::placeSidekicks(Character *player)
{
    Space *heroSpace = board.getSpace(player->getPosition());

    const vector<Zone> &zones = heroSpace->getZones();

    vector<Character *> sidekicks = player->getSidekicks();

    for (Character *sidekick : sidekicks)
    {
        vector<Space *> availableSpaces;

        for (Zone zone : zones)
        {
            vector<Space *> spacesInZone = board.getSpacesInZone(zone);

            for (Space *space : spacesInZone)
            {
                if (!space->isOccupied())
                {
                    bool exists = false;

                    for (Space *s : availableSpaces)
                    {
                        if (s->getId() == space->getId())
                        {
                            exists = true;
                            break;
                        }
                    }

                    if (!exists)
                    {
                        availableSpaces.push_back(space);
                    }
                }
            }
        }

        cout << endl;

        cout << "Choose a space for " << sidekick->getName() << ":" << endl;

        for (Space *space : availableSpaces)
        {
            cout << space->getId() << " ";
        }

        cout << endl;

        int choice;

        while (true)
        {
            try
            {
                cin >> choice;
                if (cin.fail())
                {
                    cin.clear();
                    cin.ignore(1000, '\n');
                    throw invalid_argument("Please enter a number.");
                }

                bool valid = false;

                for (Space *space : availableSpaces)
                {
                    if (space->getId() == choice)
                    {
                        valid = true;
                        break;
                    }
                }

                cout << endl;
                if (!valid)
                {
                    throw invalid_argument("Invalid space.");
                }

                placeCharacter(sidekick, choice);
                break;
            }
            catch (const exception &e)
            {
                cout << e.what() << endl;
            }
        }
    }
}
bool Game::isAdjacent(int from, int to) const
{
    const Space *current = board.getSpace(from);

    for (const Space *space : current->getAdjacentSpaces())
    {
        if (space->getId() == to)
        {
            return true;
        }
    }

    return false;
}
Character *Game::getHeroOwner(Character *fighter) const
{
    if (fighter == nullptr)
    {
        return nullptr;
    }

    if (fighter == player1)
    {
        return player1;
    }

    if (fighter == player2)
    {
        return player2;
    }

    for (Character *sidekick : player1->getSidekicks())
    {
        if (sidekick == fighter)
        {
            return player1;
        }
    }

    for (Character *sidekick : player2->getSidekicks())
    {
        if (sidekick == fighter)
        {
            return player2;
        }
    }

    return nullptr;
}
void Game::applyFatigueDamage(Character *fighter)
{
    Character *hero = getHeroOwner(fighter);

    if (hero == nullptr)
    {
        cout << "Could not determine fighter owner.\n";
        return;
    }

    vector<Character *> fighters = getFighters(hero);

    for (Character *current : fighters)
    {
        if (current != nullptr && current->isAlive())
        {
            current->takeDamage(2);

            cout << current->getName()
                 << " takes 2 fatigue damage.\n";
        }
    }

    removeDefeatedSidekicks();
}
void Game::maneuver(Character *character)
{
    if (character == nullptr)
    {
        throw invalid_argument("Character is null.");
    }

    try
    {
        character->drawCard();
        cout << character->getName() << " drew a card." << endl;
    }
    catch (const exception &e)
    {
        cout << "Cannot draw card: "
             << e.what()
             << endl;

        applyFatigueDamage(character);

        return;
    }

    return;

    cout << "Do you want to move? (1: Yes, 0: No): ";

    int choice;
    cin >> choice;

    if (choice == 0)
    {
        return;
    }

    if (choice != 1)
    {
        throw invalid_argument("Invalid movement choice.");
    }

    cout << "Enter destination: ";

    int destination;
    cin >> destination;

    if (canMove(character, destination))
    {
        moveCharacter(character, destination);
        cout << "Move successful!" << endl;
    }
    else
    {
        cout << "Invalid move." << endl;
    }
}
void Game::scheme(Character *character)
{
    if (character == nullptr)
    {
        throw invalid_argument("Character is null.");
    }

    character->showHand();

    cout << "Choose a Scheme card index: ";

    int index;
    cin >> index;

    Card card = character->getCardFromHand(index);
    if (card.getFighterType() == FighterType::Sidekick)
    {
        bool livingSidekick = false;

        for (Character *sidekick : character->getSidekicks())
        {
            if (sidekick != nullptr && sidekick->isAlive())
            {
                livingSidekick = true;
                break;
            }
        }

        if (!livingSidekick)
        {
            cout << "The sidekick is defeated. " << "This card cannot be played as a Scheme.\n";
            return;
        }
    }

    if (card.getType() != CardType::Scheme)
    {
        cout << "Selected card is not a Scheme card.\n";
        return;
    }

    cout << character->getName()
         << " played "
         << card.getName()
         << endl;

    switch (card.getEffect())
    {
    case CardEffect::RaveningSeduction:
    {
        Dracula *dracula = dynamic_cast<Dracula *>(character);

        if (dracula == nullptr)
        {
            cout << "Only Dracula can use Ravening Seduction.\n";
            break;
        }

        vector<Character *> fighters;

        fighters.push_back(player1);

        for (Character *sidekick : player1->getSidekicks())
        {
            if (sidekick != nullptr)
            {
                fighters.push_back(sidekick);
            }
        }

        fighters.push_back(player2);

        for (Character *sidekick : player2->getSidekicks())
        {
            if (sidekick != nullptr)
            {
                fighters.push_back(sidekick);
            }
        }

        cout << "\nChoose a fighter to move:\n";

        for (int i = 0; i < static_cast<int>(fighters.size()); i++)
        {
            if (fighters[i] != nullptr && fighters[i]->isAlive())
            {
                cout << i << ". " << fighters[i]->getName() << " - Space " << fighters[i]->getPosition() << endl;
            }
        }

        int fighterIndex;
        cin >> fighterIndex;

        if (fighterIndex < 0 ||
            fighterIndex >= static_cast<int>(fighters.size()) ||
            fighters[fighterIndex] == nullptr ||
            !fighters[fighterIndex]->isAlive())
        {
            cout << "Invalid fighter.\n";
            break;
        }

        Character *target = fighters[fighterIndex];

        cout << "Enter destination for " << target->getName() << " (up to 2 spaces, -1 to skip movement): ";

        int destination;
        cin >> destination;

        if (destination != -1)
        {
            if (!canMoveByCard(target, destination, 2))
            {
                cout << "Invalid destination.\n";
                break;
            }

            Space *destinationSpace = board.getSpace(destination);

            if (destinationSpace->isOccupied())
            {
                cout << "Destination is occupied.\n";
                break;
            }

            if (target->getPosition() != 0)
            {
                Space *currentSpace = board.getSpace(target->getPosition());

                currentSpace->setOccupant(nullptr);
            }

            destinationSpace->setOccupant(target);
            target->setPosition(destination);

            cout << target->getName() << " moved to space :" << destination << endl;
        }

        int adjacentSisters = 0;

        for (Sister &sister : dracula->getSisters())
        {
            if (!sister.isAlive())
            {
                continue;
            }

            if (sister.getPosition() == 0)
            {
                continue;
            }

            if (isAdjacent(
                    sister.getPosition(),
                    target->getPosition()))
            {
                adjacentSisters++;
            }
        }

        target->takeDamage(adjacentSisters);

        cout << target->getName() << " took " << adjacentSisters << " damage from adjacent Sisters.\n";

        break;
    }
    case CardEffect::PreyUpon:
    {
        Dracula *dracula = dynamic_cast<Dracula *>(character);

        if (dracula == nullptr)
        {
            cout << "Only Dracula can use Prey Upon.\n";
            break;
        }

        Character *opponentHero;

        if (character == player1)
        {
            opponentHero = player2;
        }
        else
        {
            opponentHero = player1;
        }

        vector<Character *> enemyFighters;

        enemyFighters.push_back(opponentHero);

        for (Character *sidekick : opponentHero->getSidekicks())
        {
            if (sidekick != nullptr)
            {
                enemyFighters.push_back(sidekick);
            }
        }

        int totalDamageDealt = 0;

        for (Character *fighter : enemyFighters)
        {
            if (fighter == nullptr || !fighter->isAlive())
            {
                continue;
            }

            if (isAdjacent(dracula->getPosition(), fighter->getPosition()))
            {
                fighter->takeDamage(1);
                totalDamageDealt++;

                cout << fighter->getName() << " took 1 damage.\n";
            }
        }

        dracula->heal(totalDamageDealt);

        cout << "Dracula healed " << totalDamageDealt << " HP.\n";

        break;
    }

    case CardEffect::EliminateTheImpossible:
    {
        Character *opponent;

        if (character == player1)
        {
            opponent = player2;
        }
        else
        {
            opponent = player1;
        }

        if (opponent == nullptr)
        {
            cout << "Opponent not found.\n";
            break;
        }

        if (opponent->getHandSize() == 0)
        {
            cout << "Opponent has no cards.\n";
            break;
        }

        cout << "\nOpponent's hand:\n";
        opponent->showHand();

        cout << "Choose a card index to discard: ";

        int selectedIndex;
        cin >> selectedIndex;

        if (selectedIndex < 0 ||
            selectedIndex >= opponent->getHandSize())
        {
            cout << "Invalid card index.\n";
            break;
        }

        Card discardedCard =
            opponent->getCardFromHand(selectedIndex);

        opponent->discardCard(selectedIndex);

        cout << discardedCard.getName()
             << " was discarded from "
             << opponent->getName()
             << "'s hand.\n";

        break;
    }

    case CardEffect::AidDrWatson:
    {
        Sherlock *sherlock = dynamic_cast<Sherlock *>(character);

        if (sherlock == nullptr)
        {
            break;
        }

        vector<Character *> sidekicks = sherlock->getSidekicks();

        for (Character *sidekick : sidekicks)
        {
            if (sidekick->getName() == "Dr. Watson")
            {
                cout << "Moving Dr. Watson near Sherlock.\n";

                Space *heroSpace = board.getSpace(character->getPosition());

                vector<Space *> adjacent = heroSpace->getAdjacentSpaces();

                cout << "Choose a space for Dr. Watson:\n";

                for (Space *space : adjacent)
                {
                    if (!space->isOccupied())
                    {
                        cout << space->getId() << " ";
                    }
                }

                cout << endl;

                int destination;
                cin >> destination;

                moveCharacter(sidekick, destination);

                break;
            }
        }

        character->heal(1);
        try
        {
            character->drawCard();

            cout << character->getName() << " drew 1 card.\n";
        }
        catch (const exception &e)
        {
            cout << "Cannot draw card: " << e.what() << endl;

            applyFatigueDamage(character);
        }

        cout << "Sherlock healed and drew a card.\n";

        break;
    }
    case CardEffect::MasterOfDisguise:
    {
        Sherlock *sherlock =
            dynamic_cast<Sherlock *>(character);

        if (sherlock == nullptr)
        {
            cout << "Only Sherlock can use Master of Disguise.\n";
            break;
        }

        Character *opponent;

        if (character == player1)
        {
            opponent = player2;
        }
        else
        {
            opponent = player1;
        }

        if (opponent == nullptr)
        {
            cout << "Opponent not found.\n";
            break;
        }

        int sherlockPosition = sherlock->getPosition();
        int opponentPosition = opponent->getPosition();

        if (sherlockPosition == 0 || opponentPosition == 0)
        {
            cout << "Both fighters must be on the board.\n";
            break;
        }

        Space *sherlockSpace = board.getSpace(sherlockPosition);

        Space *opponentSpace = board.getSpace(opponentPosition);

        sherlockSpace->setOccupant(opponent);
        opponentSpace->setOccupant(sherlock);

        sherlock->setPosition(opponentPosition);
        opponent->setPosition(sherlockPosition);

        opponent->takeDamage(1);

        cout << "Sherlock and " << opponent->getName() << " swapped positions.\n";

        cout << opponent->getName() << " took 1 damage.\n";

        break;
    }
    default:

        break;
    }

    character->discardCard(index);
}

bool Game::attack(Character *attacker)
{
    if (attacker == nullptr)
    {
        throw invalid_argument("Attacker is null.");
    }

    cout << "\n.......... Attack .........\n";
    cout << "Attacker: "
         << attacker->getName()
         << endl;

    vector<Character *> targets =
        getAttackableTargets(attacker);

    if (targets.empty())
    {
        cout << "No valid targets.\n";
        return false;
    }

    cout << "Choose target:\n";

    for (int i = 0;
         i < static_cast<int>(targets.size());
         i++)
    {
        cout << i << ". "
             << targets[i]->getName()
             << " (HP: "
             << targets[i]->getHealth()
             << ", Space: "
             << targets[i]->getPosition()
             << ")\n";
    }

    int targetIndex;

    while (true)
    {
        cout << "Choice: ";
        cin >> targetIndex;

        if (!cin.fail() &&
            targetIndex >= 0 &&
            targetIndex <
                static_cast<int>(targets.size()))
        {
            break;
        }

        cin.clear();
        cin.ignore(1000, '\n');

        cout << "Invalid target choice.\n";
    }

    Character *defender =
        targets[targetIndex];

    if (defender == nullptr ||
        !defender->isAlive() ||
        defender->getPosition() == 0)
    {
        cout << "No valid target.\n";
        return false;
    }

    cout << "Target: "
         << defender->getName()
         << endl;

    int attackIndex;

    try
    {
        attackIndex =
            chooseAttackCard(attacker);
    }
    catch (const exception &e)
    {
        cout << "Attack cancelled: "
             << e.what()
             << endl;

        return false;
    }

    int defenseIndex = -1;

    if (defender->getHandSize() > 0)
    {
        try
        {
            defenseIndex =
                chooseDefenseCard(defender);
        }
        catch (const exception &e)
        {
            cout << "Defense selection failed: "
                 << e.what()
                 << endl;

            return false;
        }
    }

    Card attackCard =
        attacker->getCardFromHand(attackIndex);

    Card *defenseCard = nullptr;

    if (defenseIndex != -1)
    {
        defenseCard = new Card(
            defender->getCardFromHand(
                defenseIndex));
    }

    resolveCombat(
        attacker,
        defender,
        attackCard,
        defenseCard);

    delete defenseCard;
    defenseCard = nullptr;

    attacker->discardCard(attackIndex);

    if (defenseIndex != -1)
    {
        defender->discardCard(defenseIndex);
    }

    return true;
}

void Game::applyImmediatelyEffect(
    Character *player,
    Character *opponent,
    const Card &card)
{

    switch (card.getEffect())
    {
    case CardEffect::Feint:
    {
        effectsCancelled = true;

        cout << player->getName() << " played Feint. Opponent's card effects are cancelled.\n";

        break;
    }

    case CardEffect::MistForm:
    {
        cout << "Choose destination: ";

        int destination;
        cin >> destination;

        if (destination < 1 ||
            destination > board.getNumberOfSpaces())
        {
            cout << "Invalid space.\n";
            break;
        }

        Space *target = board.getSpace(destination);

        if (target->isOccupied())
        {
            cout << "Space is occupied.\n";
            break;
        }

        board.getSpace(player->getPosition())->setOccupant(nullptr);

        player->setPosition(destination);

        target->setOccupant(player);

        actionsRemaining++;

        cout << "Mist Form activated.\n";

        break;
    }

    default:
        break;
    }
}
void Game::applyDuringCombatEffects(
    const Card &attackCard,
    const Card *defenseCard,
    int &attackValue,
    int &defenseValue)
{
    if (effectsCancelled)
    {
        return;
    }

    if (attackCard.getEffect() == CardEffect::DeduceStrategy)
    {
        if (defenseCard == nullptr)
        {
            cout << "Opponent did not play a defense card.\n";
        }
        else
        {
            cout << "Use Deduce Strategy? (1 = Yes, 0 = No): ";

            int choice;
            cin >> choice;

            if (choice == 1)
            {
                defenseValue = defenseCard->getBoost();

                cout << "Opponent defense value changed to "
                     << defenseValue
                     << ".\n";
            }
        }
    }

    if (defenseCard != nullptr &&
        defenseCard->getEffect() == CardEffect::DeduceStrategy)
    {
        cout << "Use Deduce Strategy? (1 = Yes, 0 = No): ";

        int choice;
        cin >> choice;

        if (choice == 1)
        {
            attackValue = attackCard.getBoost();

            cout << "Opponent attack value changed to "
                 << attackValue
                 << ".\n";
        }
    }
}
void Game::applyAfterCombatEffect(
    Character *player,
    Character *opponent,
    const Card &card,
    bool playerWon)
{
    {

        switch (card.getEffect())
        {
        case CardEffect::Exploit:
        {
            try
            {
                player->drawCard();

                cout << player->getName() << " drew 1 card.\n";
            }
            catch (const exception &e)
            {
                cout << "Cannot draw card: " << e.what() << endl;
                applyFatigueDamage(player);
            }

            cout << player->getName() << " drew a card.\n";

            break;
        }

        case CardEffect::Dash:
        {
            moveCharacterByEffect(player, 3);
            break;
        }

        case CardEffect::ThirstForSustenance:
        {
            if (!playerWon)
            {
                cout << "Thirst for Sustenance did not activate because Dracula lost the combat.\n";
                break;
            }

            Dracula *dracula = dynamic_cast<Dracula *>(player);

            if (dracula == nullptr)
            {
                break;
            }

            Space *opponentSpace =
                board.getSpace(opponent->getPosition());

            vector<Space *> adjacentSpaces =
                opponentSpace->getAdjacentSpaces();

            vector<Space *> availableSpaces;

            for (Space *space : adjacentSpaces)
            {
                if (space != nullptr && !space->isOccupied())
                {
                    availableSpaces.push_back(space);
                }
            }

            if (availableSpaces.empty())
            {
                cout << "No empty adjacent space available.\n";
                break;
            }

            cout << "Choose a space next to "
                 << opponent->getName()
                 << ":\n";

            for (Space *space : availableSpaces)
            {
                cout << space->getId() << " ";
            }

            cout << endl;

            int destination;
            cin >> destination;

            bool valid = false;

            for (Space *space : availableSpaces)
            {
                if (space->getId() == destination)
                {
                    valid = true;
                    break;
                }
            }

            if (!valid)
            {
                cout << "Invalid destination.\n";
                break;
            }

            Space *oldSpace =
                board.getSpace(dracula->getPosition());

            oldSpace->setOccupant(nullptr);

            Space *newSpace =
                board.getSpace(destination);

            newSpace->setOccupant(dracula);
            dracula->setPosition(destination);

            cout << "Dracula moved next to " << opponent->getName() << endl;

            break;
        }
        case CardEffect::TheGameIsAfoot:
        {
            cout << "The Game Is Afoot activated.\n";

            int destination;

            cout << "Enter destination for Sherlock (up to 3 spaces): ";
            cin >> destination;

            if (canMoveByCard(player, destination, 3))
            {
                moveCharacter(player, destination);
            }
            else
            {
                cout << "Invalid destination.\n";
            }

            break;
        }

        case CardEffect::MasterOfDisguise:
        {
            int sherlockPos = player->getPosition();
            int enemyPos = opponent->getPosition();

            board.getSpace(sherlockPos)->setOccupant(opponent);
            board.getSpace(enemyPos)->setOccupant(player);

            player->setPosition(enemyPos);
            opponent->setPosition(sherlockPos);

            opponent->takeDamage(1);

            break;
        }
        case CardEffect::StudyMethods:
        {
            opponent->showHand();
            break;
        }
        case CardEffect::EducationNeverEnds:
        {
            try
            {
                opponent->drawCard();
                cout << opponent->getName() << " drew 1 card.\n";
            }
            catch (const exception &e)
            {
                cout << "Cannot draw card: " << e.what() << endl;

                applyFatigueDamage(opponent);
            }
            break;
        }
        case CardEffect::CounterPunch:
        {
            if (isAdjacent(player->getPosition(), opponent->getPosition()))
            {
                opponent->takeDamage(2);
            }

            break;
        }
        case CardEffect::FixedPointInAChangingAge:
        {
            Sherlock *sherlock = nullptr;
            Character *watson = nullptr;

            sherlock = dynamic_cast<Sherlock *>(player);

            if (sherlock != nullptr)
            {
                for (Character *sidekick : sherlock->getSidekicks())
                {
                    if (sidekick != nullptr &&
                        sidekick->getName() == "Dr. Watson")
                    {
                        watson = sidekick;
                        break;
                    }
                }
            }
            else
            {

                if (player->getName() == "Dr. Watson")
                {
                    watson = player;

                    sherlock = dynamic_cast<Sherlock *>(player1);

                    if (sherlock == nullptr)
                    {
                        sherlock = dynamic_cast<Sherlock *>(player2);
                    }
                }
            }

            if (sherlock == nullptr || watson == nullptr)
            {
                cout << "Sherlock or Dr. Watson not found.\n";
                break;
            }

            if (isAdjacent(
                    sherlock->getPosition(),
                    watson->getPosition()))
            {
                sherlock->heal(1);
                watson->heal(1);

                cout << "Sherlock and Dr. Watson healed 1 HP.\n";
            }
            else
            {
                cout << "Dr. Watson is not adjacent to Sherlock.\n";
            }

            break;
        }

        default:
            break;
        }
    }
}
vector<Character *> Game::getAttackableTargets(
    Character *attacker)
{
    vector<Character *> targets;

    if (attacker == nullptr ||
        !attacker->isAlive() ||
        attacker->getPosition() == 0)
    {
        return targets;
    }

    Character *opponentHero = getOpponent();

    if (opponentHero == nullptr)
    {
        return targets;
    }

    vector<Character *> enemyFighters =
        getFighters(opponentHero);

    for (Character *target : enemyFighters)
    {
        if (target == nullptr ||
            !target->isAlive() ||
            target->getPosition() == 0)
        {
            continue;
        }

        bool canAttack = false;

        if (attacker->getAttackRange() ==
            AttackRange::Melee)
        {
            canAttack = isAdjacent(
                attacker->getPosition(),
                target->getPosition());
        }
        else
        {
            canAttack = shareZone(
                attacker,
                target);
        }

        if (canAttack)
        {
            targets.push_back(target);
        }
    }

    return targets;
}

int Game::chooseAttackCard(Character *attacker)
{
    if (attacker == nullptr)
    {
        throw invalid_argument("Attacker is null.");
    }

    if (!attacker->isAlive() ||
        attacker->getPosition() == 0)
    {
        throw runtime_error("This fighter cannot attack.");
    }

    attacker->showHand();

    cout << "Choose attack card index: ";

    int index;

    while (true)
    {
        cin >> index;

        if (!cin.fail() &&
            index >= 0 &&
            index < attacker->getHandSize())
        {
            break;
        }

        cin.clear();
        cin.ignore(1000, '\n');

        cout << "Invalid card index. Try again: ";
    }

    Card card = attacker->getCardFromHand(index);

    if (card.getType() != CardType::Attack && card.getType() != CardType::Versatile)
    {
        throw runtime_error("This card cannot be used for attack.");
    }

    if (!canFighterUseCard(attacker, card))
    {
        throw runtime_error("This fighter cannot use this card.");
    }

    return index;
}

int Game::chooseDefenseCard(Character *defender)
{
    defender->showHand();

    cout << "Choose defense card index "
         << "(-1 for no defense): ";

    int index;
    cin >> index;

    if (index == -1)
    {
        return -1;
    }

    Card card =
        defender->getCardFromHand(index);

    if (card.getType() != CardType::Defense &&
        card.getType() != CardType::Versatile)
    {
        throw runtime_error(
            "This card cannot be used for defense.");
    }

    if (!canFighterUseCard(defender, card))
    {
        throw runtime_error(
            "This fighter cannot use this card.");
    }

    return index;
}

bool Game::canFighterUseCard(
    Character *fighter,
    const Card &card) const
{
    if (fighter == nullptr || !fighter->isAlive())
    {
        return false;
    }

    if (card.getFighterType() == FighterType::Any)
    {
        return true;
    }

    bool isSidekick =
        fighter->getName() == "Dr. Watson" ||
        fighter->getName() == "Sister";

    if (card.getFighterType() == FighterType::Hero)
    {
        return !isSidekick;
    }

    if (card.getFighterType() == FighterType::Sidekick)
    {
        return isSidekick;
    }

    return false;
}

bool Game::hasLivingFighterForCard(
    Character *hero,
    const Card &card) const
{
    if (hero == nullptr)
    {
        return false;
    }

    if (card.getFighterType() == FighterType::Any ||
        card.getFighterType() == FighterType::Hero)
    {
        return hero->isAlive();
    }

    if (card.getFighterType() == FighterType::Sidekick)
    {
        for (Character *sidekick : hero->getSidekicks())
        {
            if (sidekick != nullptr &&
                sidekick->isAlive() &&
                sidekick->getPosition() != 0)
            {
                return true;
            }
        }

        return false;
    }

    return false;
}
bool Game::performAction(Character *character)
{
    if (character == nullptr)
    {
        throw invalid_argument("Character is null.");
    }

    cout << "Choose Action:\n";
    cout << "1. Maneuver\n";
    cout << "2. Scheme\n";
    cout << "3. Attack\n";

    int choice;
    cin >> choice;

    switch (choice)
    {
    case 1:
        maneuver(character);
        return true;

    case 2:
        scheme(character);
        return true;

    case 3:
    {
        Character *attacker = chooseAttackingFighter(character);

        return attack(attacker);
    }
    default:
        cout << "Invalid action choice.\n";
        return false;
    }
}

void Game::playTurn(Character *character)
{
    if (dynamic_cast<Dracula *>(character) != nullptr)
    {
        cout << "\nUse Dracula ability?\n";
        cout << "1. Yes\n";
        cout << "0. No\n";

        int choice;
        cin >> choice;

        if (choice == 1)
        {
            useCharacterAbility(character);
        }
    }

    cout << "\n........ " << character->getName() << "'s Turn ........\n";

    actionsRemaining = 2;

    while (actionsRemaining > 0 && !isGameOver())
    {
        cout << "\nRemaining actions: " << actionsRemaining << endl;

        bool completed = performAction(character);

        removeDefeatedSidekicks();

        if (isGameOver())
        {
            break;
        }

        if (completed)
        {
            actionsRemaining--;
        }
        else
        {
            cout << "Action was not completed. Try again.\n";
        }
    }

    if (!isGameOver())
    {
        character->discardExcessCards();
        cout << "Turn ended.\n";
    }
}

void Game::initialPlacement(bool player1IsYounger)
{
    Character *youngerPlayer;
    Character *olderPlayer;

    int youngerPlayerNumber;

    if (player1IsYounger)
    {
        youngerPlayer = player1;
        olderPlayer = player2;
        youngerPlayerNumber = 1;
    }
    else
    {
        youngerPlayer = player2;
        olderPlayer = player1;
        youngerPlayerNumber = 2;
    }
    int choice;

    while (true)
    {
        try
        {

            cout << "Player " << youngerPlayerNumber << ", choose your starting space:" << endl;
            cout << "1. Space 1" << endl;
            cout << "2. Space 2" << endl;
            cout << "Choice: ";
            cin >> choice;

            if (cin.fail())
            {
                cin.clear();
                cin.ignore(1000, '\n');
                throw invalid_argument("Please enter a number.");
            }

            if (choice != 1 && choice != 2)
            {
                throw invalid_argument("Choice must be 1 or 2.");
            }

            break;
        }
        catch (const exception &e)
        {
            cout << e.what() << endl;
        }
    }
    if (choice == 1)
    {
        placeCharacter(youngerPlayer, 2);
        placeCharacter(olderPlayer, 23);
    }
    else
    {
        placeCharacter(youngerPlayer, 23);
        placeCharacter(olderPlayer, 2);
    }
    placeSidekicks(youngerPlayer);
    placeSidekicks(olderPlayer);
}

void Game::nextTurn()
{
    if (currentPlayer == 1)
    {
        currentPlayer = 2;
    }
    else
    {
        currentPlayer = 1;
    }
}
void Game::removeFighterFromBoard(Character *fighter)
{
    if (fighter == nullptr)
    {
        return;
    }

    int position = fighter->getPosition();

    if (position == 0)
    {
        return;
    }

    Space *space = board.getSpace(position);

    if (space != nullptr &&
        space->getOccupant() == fighter)
    {
        space->setOccupant(nullptr);
    }

    fighter->setPosition(0);
}
void Game::removeDefeatedSidekicks()
{
    Character *heroes[2] = {
        player1,
        player2};

    for (Character *hero : heroes)
    {
        if (hero == nullptr)
        {
            continue;
        }

        for (Character *sidekick : hero->getSidekicks())
        {
            if (sidekick == nullptr)
            {
                continue;
            }

            if (!sidekick->isAlive() &&
                sidekick->getPosition() != 0)
            {
                cout << sidekick->getName()
                     << " was defeated and removed from the board.\n";

                removeFighterFromBoard(sidekick);
            }
        }
    }
}

bool Game::isGameOver() const
{
    if (player1 == nullptr || player2 == nullptr)
    {
        return true;
    }

    return !player1->isAlive() || !player2->isAlive();
}

Board &Game::getBoard()
{
    return board;
}

Character *Game::getCurrentPlayer() const
{
    if (currentPlayer == 1)
    {
        return player1;
    }

    return player2;
}

Character *Game::getOpponent() const
{
    if (currentPlayer == 1)
    {
        return player2;
    }

    return player1;
}
void Game::moveCharacter(Character *character, int destination)
{
    if (character == nullptr)
    {
        throw invalid_argument("Character pointer is null.");
    }

    if (destination < 1 || destination > board.getNumberOfSpaces())
    {
        throw out_of_range("Invalid destination.");
    }

    if (!canMove(character, destination))
    {
        throw runtime_error("Invalid move.");
    }

    Space *next = board.getSpace(destination);

    if (next->isOccupied())
    {
        throw runtime_error("Destination is occupied.");
    }

    if (character->getPosition() != 0)
    {
        Space *current = board.getSpace(character->getPosition());
        current->setOccupant(nullptr);
    }

    next->setOccupant(character);
    character->setPosition(destination);
}
void Game::moveCharacterByEffect(Character *character, int maxDistance)
{
    if (character == nullptr)
    {
        throw invalid_argument("Character is null.");
    }

    cout << character->getName()
         << " may move up to "
         << maxDistance
         << " spaces.\n";

    cout << "Enter destination (-1 to skip): ";

    int destination;
    cin >> destination;

    if (destination == -1)
    {
        return;
    }

    if (canMoveByCard(character, destination, maxDistance))
    {
        moveCharacter(character, destination);
        cout << "Move successful.\n";
    }
    else
    {
        cout << "Invalid move.\n";
    }
}
bool Game::isTeammate(Character *hero, Character *other)
{
    if (hero == other)
    {
        return true;
    }

    vector<Character *> sidekicks = hero->getSidekicks();

    for (Character *sidekick : sidekicks)
    {
        if (sidekick == other)
        {
            return true;
        }
    }

    return false;
}
bool Game::canMove(Character *character, int destination)
{
    if (character == nullptr)
    {
        return false;
    }

    if (destination < 1 || destination > board.getNumberOfSpaces())
    {
        return false;
    }

    if (character->getPosition() == destination)
    {
        return false;
    }

    queue<pair<Space *, int>> q;

    vector<bool> visited(board.getNumberOfSpaces() + 1, false);

    Space *start = board.getSpace(character->getPosition());

    q.push({start, 0});
    visited[start->getId()] = true;

    while (!q.empty())
    {
        Space *current = q.front().first;
        int distance = q.front().second;
        q.pop();

        vector<Space *> neighbors = board.getMovementNeighbors(current);

        for (Space *next : neighbors)
        {
            if (visited[next->getId()])
            {
                continue;
            }

            Character *occupant = next->getOccupant();

            if (occupant != nullptr)
            {
                if (!isTeammate(character, occupant))
                {
                    continue;
                }

                if (next->getId() == destination)
                {
                    continue;
                }
            }

            if (next->getId() == destination &&
                distance + 1 <= character->getMovement())
            {
                return true;
            }

            if (distance + 1 <= character->getMovement())
            {
                visited[next->getId()] = true;
                q.push({next, distance + 1});
            }
        }
    }

    return false;
}

void Game::resolveCombat(
    Character *attacker,
    Character *defender,
    const Card &attackCard,
    const Card *defenseCard)
{
    bool attackEffectCancelled = false;
    bool defenseEffectCancelled = false;

    int attackValue = attackCard.getAttackValue();
    int defenseValue = 0;

    if (defenseCard != nullptr)
    {
        defenseValue = defenseCard->getDefenseValue();
    }
    if (attackCard.getEffect() == CardEffect::Feint &&
        defenseCard != nullptr)
    {
        Sherlock *defendingSherlock =
            dynamic_cast<Sherlock *>(defender);

        if (defendingSherlock == nullptr ||
            defendingSherlock->canBeDisabledByEffect(*defenseCard))
        {
            defenseEffectCancelled = true;
        }
    }

    if (defenseCard != nullptr &&
        defenseCard->getEffect() == CardEffect::Feint)
    {
        Sherlock *attackingSherlock =
            dynamic_cast<Sherlock *>(attacker);

        if (attackingSherlock == nullptr ||
            attackingSherlock->canBeDisabledByEffect(attackCard))
        {
            attackEffectCancelled = true;
        }
    }

    applyDuringCombatEffects(
        attackCard,
        defenseCard,
        attackValue,
        defenseValue);

    if (defenseCard != nullptr &&
        !defenseEffectCancelled)
    {
        applyImmediatelyEffect(
            defender,
            attacker,
            *defenseCard);
    }

    if (!attackEffectCancelled)
    {
        applyImmediatelyEffect(
            attacker,
            defender,
            attackCard);

        modifyAttackValue(
            attacker,
            defender,
            attackCard,
            attackValue);
    }

    if (defenseCard != nullptr &&
        !defenseEffectCancelled)
    {
        modifyDefenseValue(
            defender,
            attacker,
            *defenseCard,
            attackCard,
            defenseValue);
    }

    int damage = attackValue - defenseValue;

    if (damage < 0)
    {
        damage = 0;
    }

    bool attackerWon = (damage > 0);

    defender->takeDamage(damage);

    cout << defender->getName() << " took " << damage << " damage." << endl;

    if (!attackEffectCancelled)
    {
        applyAfterCombatEffect(
            attacker,
            defender,
            attackCard,
            attackerWon);
    }

    if (defenseCard != nullptr &&
        !defenseEffectCancelled)
    {
        applyAfterCombatEffect(
            defender,
            attacker,
            *defenseCard,
            !attackerWon);
    }
    removeDefeatedSidekicks();
}
void Game::modifyDefenseValue(
    Character *defender,
    Character *attacker,
    const Card &defenseCard,
    const Card &attackCard,
    int &defenseValue)
{
    switch (defenseCard.getEffect())
    {
    case CardEffect::LookIntoMyEyes:
    {
        defenseValue += attackCard.getBoost();

        cout << "Look Into My Eyes: +" << attackCard.getBoost() << " defense.\n";

        break;
    }

    default:
        break;
    }
}
void Game::modifyAttackValue(
    Character *attacker,
    Character *defender,
    const Card &attackCard,
    int &attackValue)
{
    switch (attackCard.getEffect())
    {
    case CardEffect::Beastform:
    {
        cout << "Beastform activated.\n";

        int count = 0;

        while (true)
        {
            attacker->showHand();

            cout << "How many cards do you want to discard? ";
            cin >> count;

            if (count < 0 ||
                count > attacker->getHandSize())
            {
                cout << "Invalid amount.\n";
            }
            else
            {
                break;
            }
        }

        for (int i = 0; i < count; i++)
        {
            attacker->showHand();

            cout << "Choose card index to discard: ";

            int index;
            cin >> index;

            while (index < 0 || index >= attacker->getHandSize())
            {
                cout << "Invalid card index. Try again: ";
                cin >> index;
            }

            Card discarded = attacker->getCardFromHand(index);

            attacker->discardCard(index);

            cout << discarded.getName() << " was discarded.\n";
        }
        attackValue += count;
        break;
    }

    case CardEffect::FeedingFrenzy:
    {
        Dracula *dracula =
            dynamic_cast<Dracula *>(attacker);

        if (dracula != nullptr)
        {
            int bonus = 0;

            for (Sister &sister : dracula->getSisters())
            {
                if (!sister.isAlive())
                {
                    continue;
                }

                if (sister.getPosition() == 0)
                {
                    continue;
                }

                if (isAdjacent(
                        sister.getPosition(),
                        defender->getPosition()))
                {
                    bonus++;
                }
            }

            attackValue += bonus;

            cout << "Feeding Frenzy: +" << bonus << " attack.\n";
        }

        break;
    }

    case CardEffect::Ambush:
    {
        if (defender->getHandSize() == 0)
        {
            break;
        }

        int randomIndex =
            rand() % defender->getHandSize();

        Card discarded =
            defender->getCardFromHand(randomIndex);

        defender->discardCard(randomIndex);

        attackValue += discarded.getBoost();

        cout << defender->getName()
             << " discarded "
             << discarded.getName()
             << ".\n";

        cout << "Ambush: +"
             << discarded.getBoost()
             << " attack.\n";

        break;
    }

    default:
        break;
    }
}
bool Game::canMoveByCard(Character *character, int destination, int maxDistance)
{
    if (character == nullptr)
    {
        return false;
    }

    if (destination < 1 || destination > board.getNumberOfSpaces())
    {
        return false;
    }

    if (character->getPosition() == destination)
    {
        return false;
    }

    queue<pair<Space *, int>> q;

    vector<bool> visited(board.getNumberOfSpaces() + 1, false);

    Space *start = board.getSpace(character->getPosition());

    q.push({start, 0});
    visited[start->getId()] = true;

    while (!q.empty())
    {
        Space *current = q.front().first;
        int distance = q.front().second;
        q.pop();

        vector<Space *> neighbors = board.getMovementNeighbors(current);

        for (Space *next : neighbors)
        {
            if (visited[next->getId()])
            {
                continue;
            }

            Character *occupant = next->getOccupant();

            if (occupant != nullptr)
            {
                if (!isTeammate(character, occupant))
                {
                    continue;
                }

                if (next->getId() == destination)
                {
                    continue;
                }
            }

            if (next->getId() == destination &&
                distance + 1 <= maxDistance)
            {
                return true;
            }

            if (distance + 1 <= maxDistance)
            {
                visited[next->getId()] = true;
                q.push({next, distance + 1});
            }
        }
    }

    return false;
}
void Game::useCharacterAbility(Character *character)
{
    Dracula *dracula = dynamic_cast<Dracula *>(character);

    if (dracula == nullptr)
    {
        cout << "This character is not Dracula.\n";
        return;
    }

    vector<Character *> allFighters;

    allFighters.push_back(player1);
    allFighters.push_back(player2);

    for (Character *sidekick : player1->getSidekicks())
    {
        if (sidekick != nullptr)
        {
            allFighters.push_back(sidekick);
        }
    }

    for (Character *sidekick : player2->getSidekicks())
    {
        if (sidekick != nullptr)
        {
            allFighters.push_back(sidekick);
        }
    }

    vector<Character *> validTargets;

    for (Character *fighter : allFighters)
    {
        if (fighter == nullptr ||
            fighter == dracula ||
            !fighter->isAlive())
        {
            continue;
        }

        if (isAdjacent(
                dracula->getPosition(),
                fighter->getPosition()))
        {
            validTargets.push_back(fighter);
        }
    }

    if (validTargets.empty())
    {
        cout << "No adjacent fighter available.\n";
        return;
    }

    cout << "Choose a fighter to damage:\n";

    for (int i = 0; i < static_cast<int>(validTargets.size()); i++)
    {
        cout << i << ". " << validTargets[i]->getName() << " HP: " << validTargets[i]->getHealth() << endl;
    }

    int targetIndex;
    cin >> targetIndex;

    if (targetIndex < 0 ||
        targetIndex >= static_cast<int>(validTargets.size()))
    {
        cout << "Invalid target.\n";
        return;
    }

    Character *target = validTargets[targetIndex];

    target->takeDamage(1);

    cout << target->getName() << " took 1 damage.\n";
    try
    {
        dracula->drawCard();

        cout << "Dracula drew 1 card.\n";
    }
    catch (const exception &e)
    {
        cout << "Could not draw card: " << e.what() << endl;
        applyFatigueDamage(dracula);
    }
}
vector<Character *> Game::getFighters(Character *hero)
{
    vector<Character *> fighters;

    if (hero == nullptr)
    {
        return fighters;
    }

    if (hero->isAlive() &&
        hero->getPosition() != 0)
    {
        fighters.push_back(hero);
    }

    for (Character *sidekick : hero->getSidekicks())
    {
        if (sidekick != nullptr &&
            sidekick->isAlive() &&
            sidekick->getPosition() != 0)
        {
            fighters.push_back(sidekick);
        }
    }

    return fighters;
}
Character *Game::chooseAttackingFighter(Character *hero)
{
    vector<Character *> fighters = getFighters(hero);

    if (fighters.empty())
    {
        throw runtime_error("No available fighter.");
    }

    cout << "\nChoose attacking fighter:\n";

    for (int i = 0;
         i < static_cast<int>(fighters.size());
         i++)
    {
        cout << i << ". "
             << fighters[i]->getName()
             << " (HP: "
             << fighters[i]->getHealth()
             << ", Space: "
             << fighters[i]->getPosition()
             << ")\n";
    }

    int choice;

    while (true)
    {
        cout << "Choice: ";
        cin >> choice;

        if (!cin.fail() &&
            choice >= 0 &&
            choice < static_cast<int>(fighters.size()))
        {
            return fighters[choice];
        }

        cin.clear();
        cin.ignore(1000, '\n');

        cout << "Invalid fighter choice.\n";
    }
}
bool Game::shareZone(
    Character *first,
    Character *second) const
{
    if (first == nullptr || second == nullptr)
    {
        return false;
    }

    if (first->getPosition() == 0 ||
        second->getPosition() == 0)
    {
        return false;
    }

    const Space *firstSpace =
        board.getSpace(first->getPosition());

    const Space *secondSpace =
        board.getSpace(second->getPosition());

    for (Zone firstZone : firstSpace->getZones())
    {
        for (Zone secondZone : secondSpace->getZones())
        {
            if (firstZone == secondZone)
            {
                return true;
            }
        }
    }

    return false;
}