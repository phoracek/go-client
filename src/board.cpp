#include "board.h"

#include <SDL_log.h>

Board::Board()
        : on_turn_('0'), on_turn_changed_(true), winner_('0'),
          board_size_(-1),
          net_("https://responder-horacek.rhcloud.com") {
    while ( net_.send("GET", "/try_to_respond") != "hello") {
        SDL_Log("connecting to server");
    }
    // TODO: error checking, maybe create single function for init?
}

Board::~Board() {

}


void Board::createMatch() {
    match_id_ = net_.send("PUT", "/game/match");
    local_player_id_ = '1';

    updateBoard();
}


bool Board::joinMatch(std::string match_id) {
    if (match_id.size() == 32) {
        match_id_ = match_id;
        local_player_id_ = '2';

        updateBoard();

        return true;
    } else {
        return false;
    }
}


std::string Board::getMatchID() {
    return match_id_;
}

void Board::updateBoardThreaded() {
    std::thread t([&] {
        updateBoard();
    });
    t.detach();
}

void Board::updateBoard() {
    // acquire the board from server
    std::string game_board = net_.send("GET",
                                       "/game/" + match_id_ + "/board");

    if (board_size_ == -1) {
        board_size_ = int(std::sqrt(game_board.length()));
    }

    board_updating.lock();
    // TODO dont do it all again
    board_.clear();
    for (int i = 0; i < board_size_; i++) {
        board_.push_back(std::vector<char>());
        for (int j = 0; j < board_size_; j++) {
            board_[i].push_back(-1);
            board_[i][j] = game_board[i * (board_size_ + 1) + j];
        }
    }
    board_updating.unlock();
}

std::vector<std::vector<char>> Board::getBoard() {
    std::lock_guard<std::mutex> lg(board_updating);
    return board_;
}


bool Board::putStone(int j, int i) {
    if (onTurn()) {
        std::thread t([&, i, j]() {
            if (i >= 0 and i < board_size_ and
                j >= 0 and j < board_size_) {
                std::string call =
                        "stone=" + std::to_string(local_player_id_ - '0') +
                        "&x=" + std::to_string(i) +
                        "&y=" + std::to_string(j);
                std::string x = net_.send("PUT",
                                          "/game/" + match_id_ + "/stone",
                                          call);
                SDL_Log("put stone: %s", x.c_str());

                on_turn_ = false;
            }
        });
        t.detach();
        return true;
    }
    return false;
}


bool Board::passTurn() {
    if (onTurn()) {
        std::thread t([&]() {
            std::string call =
                    "stone=" + std::to_string(local_player_id_ - '0');
            std::string x = net_.send("PUT",
                                      "/game/" + match_id_ + "/pass_turn",
                                      call);
            SDL_Log("pass turn: %s", x.c_str());

            updateWinner();
        });
        t.detach();
        return true;
    }

    return false;
}

void Board::forfeit() {
    std::string call =
            "stone=" + std::to_string(local_player_id_ - '0');
    std::string x = net_.send("PUT",
                              "/game/" + match_id_ + "/forfeit",
                              call);
    SDL_Log("forfeit: %s", x.c_str());
}

bool Board::should_i_update() {
    if (on_turn_changed_) {
        on_turn_changed_ = false;
        return true;
    }
    return false;
}

void Board::updateWinner() {
    std::string x = net_.send("GET",
                              "/game/" + match_id_ + "/winner");
    winner_ = x.c_str()[0];
}

char Board::getWinner() {
    return winner_;
}

bool Board::localWin() {
    return getWinner() == local_player_id_;
}

void Board::updateOnTurn() {
    std::string x = net_.send("GET",
                              "/game/" + match_id_ + "/on_turn");

    char new_on_turn = x.c_str()[0];
    if (on_turn_ != new_on_turn) {
        SDL_Log("changed on turn");
        on_turn_ = new_on_turn;
        on_turn_changed_ = true;
    }
}

char Board::getOnTurn() {
    return on_turn_;
}

bool Board::onTurn() {
    return (on_turn_ == local_player_id_);
}

int Board::outgoingRequests() {
    return net_.getOutgoingRequests();
}

int Board::size() {
    return board_size_;
}