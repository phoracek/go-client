#ifndef NETWORKING_H_
#define NETWORKING_H_

#include <string>
#include <future>


class Networking {
public:
    Networking(std::string api_address);
    ~Networking();

    std::string send(std::string method, std::string path,
                     std::string data = "");
    std::string sendThreaded(std::string method, std::string path,
                             std::string data = "");

    int outgoingRequests();

private:
    std::string api_address_;

    int outgoing_requests_;
};

/*
// TODO
class GameNetworking {
public:
    GameNetworking();
    ~GameNetworking();

    std::string createMatch();
    bool setMatchID(std::string match_id);

    void getBoard(); // TODO board type?
    int getOnTurn();
    int getWinner();

    bool placeStone();
    bool passTurn();

private:
    Networking net_; // TODO maybe oop?
    std::string match_id_;

    int local_stone_; // TODO maybe???
};
*/

#endif

