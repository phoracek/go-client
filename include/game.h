#ifndef GAME_H_
#define GAME_H_

#include "graphics.h"
#include "board.h"

#include <sstream>
#include <mutex>

class Game {
public:
    Game();
    ~Game();

    void introLoop();
    void mainLoop();
    void finalLoop();

private:
    glm::vec2 pointingAt(float height, float size, float offset);

	void render();
	float last_time_, current_time_;
    std::mutex rendering_;

	void updateBoard();
    std::vector<std::shared_ptr<Renderable>> bb;


	void newMatch();
    void joinMatch();

    enum Phase {
        INTRO,
        MAIN,
        FINAL,
        BREAK
    } phase_;

    Graphics *graphics_;
    Board *board_;
    SDL_Event event_;
};

#endif // GAME_H_
