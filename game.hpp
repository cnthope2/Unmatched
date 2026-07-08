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
    void placeSidekicks(Character *player);

public:
    Game(Character *p1, Character *p2);
    void placeCharacter(Character *character, int spaceId);
    void moveCharacter(Character *character, int destination);
    void initialPlacement(bool player1IsYounger);
    void startGame();
    void playTurn();
    void nextTurn();
    bool isGameOver() const;
    Board &getBoard();
    Character *getCurrentPlayer() const;
    Character *getOpponent() const;
};

#endif