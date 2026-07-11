#ifndef GAME_HPP
#define GAME_HPP

#include "board.hpp"
#include "character.hpp"
#include "sherlock.hpp"
#include "dracula.hpp"

class Game
{
private:
    Board board;
    Character *player1;
    Character *player2;
    int currentPlayer;
    int actionsRemaining;
    bool effectsCancelled = false;
    void placeSidekicks(Character *player);
    bool isAdjacent(int from, int to) const;
    bool isTeammate(Character *hero, Character *other);

public:
    Game(Character *p1, Character *p2);
    vector<Character*> getFighters(Character* hero);
    void placeCharacter(Character *character, int spaceId);
    void moveCharacter(Character *character, int destination);
    bool canMove(Character *character, int destination);
    void initialPlacement(bool player1IsYounger);
    bool performAction(Character *character);
    void startGame();
    void nextTurn();
    bool isGameOver() const;
    void maneuver(Character *character);
    void playTurn(Character *character);
    void scheme(Character *character);
    void attack(Character *attacker);
    int chooseAttackCard(Character *attacker);
    int chooseDefenseCard(Character *defender);
    vector<Character *> getAttackableTargets(Character *attacker);
    void useCharacterAbility(Character *character);
    Board &getBoard();
    Character *getCurrentPlayer() const;
    Character *getOpponent() const;
    void resolveCombat(
        Character *attacker,
        Character *defender,
        const Card &attackCard,
        const Card *defenseCard);
    void applyImmediatelyEffect(Character *player, Character *opponent, const Card &card);
    void applyAfterCombatEffect(
        Character *player,
        Character *opponent,
        const Card &card,
        bool playerWon);
    void moveCharacterByEffect(Character *character, int maxDistance);

    void modifyDefenseValue(
        Character *defender,
        Character *attacker,
        const Card &defenseCard,
        const Card &attackCard,
        int &defenseValue);
    void applyDuringCombatEffects(
        const Card &attackCard,
        const Card *defenseCard,
        int &attackValue,
        int &defenseValue);
    bool canMoveByCard(
        Character *character,
        int destination,
        int maxDistance);
    void modifyAttackValue(
        Character *attacker,
        Character *defender,
        const Card &attackCard,
        int &attackValue);
};

#endif