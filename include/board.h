#ifndef BOARD_H_
#define BOARD_H_

#include "networking.h"
#include "camera.h"

#include <glm/glm.hpp>

#include <vector>
#include <string>
#include <cmath>
#include <thread>
#include <mutex>
#include <iostream>

class Board {
public:
    Board();
    ~Board();

    void createMatch();
    bool joinMatch(std::string match_id);
    std::string getMatchID();

    void updateBoardThreaded();
    void updateBoard();
    std::vector<std::vector<char>> getBoard();

    bool putStone(int i, int j);
    bool passTurn();
    void forfeit();

    bool should_i_update();

    void updateOnTurn();
    char getOnTurn();
    bool onTurn();
    void updateWinner();
    char getWinner();
    bool localWin();

    int outgoingRequests();

    int size();

private:
    std::string match_id_;
    char local_player_id_;

    char on_turn_;
    bool on_turn_changed_;
    char winner_;

    std::vector<std::vector<char>> board_;
    int board_size_;
    std::mutex board_updating;

    Networking net_;
};

#endif // BOARD_H_
