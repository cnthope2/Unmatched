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
    std::vector<string> actionLog;

public:
    void addLog(const string &message);
    const vector<string> &getActionLog() const;
    Game(Character *p1, Character *p2);
    vector<Character *> getFighters(Character *hero);
    Character *chooseAttackingFighter(Character *hero);
    void setupGame(bool player1IsYounger);
    void placeCharacter(Character *character, int spaceId);
    void moveCharacter(Character *character, int destination);
    Character *getHeroOwner(Character *fighter) const;
    void applyFatigueDamage(Character *fighter);
    bool canMove(Character *character, int destination);
    void initialPlacement(bool player1IsYounger);
    bool performAction(Character *character);
    void startGame(bool player1isyounger);
    void nextTurn();
    bool isGameOver() const;
    int getActionsRemaining() const;
    void removeFighterFromBoard(Character *fighter);
    void removeDefeatedSidekicks();
    void maneuver(Character *character);
    bool beginManeuver();
    bool moveDuringManeuver(Character *fighter, int destination);
    void finishManeuver();
    void playTurn(Character *character);
    void scheme(Character *character);
    bool resolveCombatFromTUI(
        Character *attacker,
        Character *defender,
        int attackCardIndex,
        int defenseCardIndex);
    bool attack(Character *attacker);
    int chooseAttackCard(Character *attacker);
    int chooseDefenseCard(Character *defender);
    bool canFighterUseCard(
        Character *fighter,
        const Card &card) const;
    bool hasLivingFighterForCard(
        Character *hero,
        const Card &card) const;
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
    bool shareZone(
        Character *first,
        Character *second) const;
    Character *getWinner() const;
};

#endif