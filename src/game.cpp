#include "game.h"

std::string data_folder = "../data/";

Game::Game() : phase_(INTRO) {
    SDL_Init(SDL_INIT_EVERYTHING);

    std::thread t([&]() {
        board_ = new Board();
    });

    graphics_ = new Graphics(640, 480);
    graphics_->getCamera()->setPosition(glm::vec3(-2.5f, 5.0f, 4.0f));
    graphics_->getCamera()->setRotation(-33.f, 0.f);

    SDL_SetRelativeMouseMode(SDL_TRUE);

    t.join();

    SDL_Log("main loop");
    introLoop();
    mainLoop();
    finalLoop();
}

Game::~Game() {
    if (graphics_ != nullptr)
        delete graphics_;
    if (board_ != nullptr)
        delete board_;

    SDL_Quit();
}

/*
auto printBoard = [](std::vector<std::vector<char>> b) {
    for (unsigned int i = 0; i < b.size(); i++) {
        for (unsigned int j = 0; j < b[i].size(); j++) {
            std::cout << b[i][j];
        }
        std::cout << '\n';
    }
    std::cout << '\n';
};
*/

void Game::introLoop() {
    // START LOOP
    if (phase_ != INTRO) {
        return;
    }
    // init

    // test call if server is online


    // CREATE RENDERABLES
    std::shared_ptr<Renderable> proto_cursor =
            graphics_->createRenderable(data_folder + "flat_hor.obj",
                                        data_folder + "hand.png");


    std::shared_ptr<Renderable> table =
            graphics_->createRenderable(data_folder + "table_empty.obj",
                                        data_folder + "table_empty.png");
    std::shared_ptr<Renderable> on_table =
            graphics_->createRenderable(data_folder + "on_table.obj",
                                        data_folder + "on_table_start.png");
    std::shared_ptr<Renderable> scenery =
            graphics_->createRenderable(data_folder + "scenery_start.obj",
                                        data_folder + "scenery_start.png");

    graphics_->getCamera()->setRotation(-33.f, 0.f);

    last_time_ = SDL_GetTicks();

    while (phase_ == INTRO) {
        current_time_ = SDL_GetTicks();


        // INPUT
        while (SDL_PollEvent(&event_)) {
            if (event_.type == SDL_KEYDOWN) {
                if (event_.key.keysym.sym == SDLK_q) { // new match
                    newMatch();
                    phase_ = MAIN;
                } else if (event_.key.keysym.sym == SDLK_w) { // join match
                    joinMatch();
                    phase_ = MAIN;
                } else if (event_.key.keysym.sym == SDLK_e) { // console join
                    std::string input_id;
                    std::cout << "Enter match_id: ";
                    std::cin >> input_id;
                    board_->joinMatch(input_id);
                    phase_ = MAIN;
                } else if (event_.key.keysym.sym == SDLK_ESCAPE) { // exit
                    phase_ = BREAK;
                }
            } else if (event_.type == SDL_QUIT) { // exit
                phase_ = BREAK;
            } else if (event_.type == SDL_MOUSEMOTION) { // mouse movement
                graphics_->getCamera()->addRotation(event_.motion.yrel,
                                                    event_.motion.xrel);
            } else if (event_.type == SDL_MOUSEBUTTONDOWN) { // action
                if (event_.button.button == SDL_BUTTON_LEFT) {
                    // position
                    glm::vec2 tar = pointingAt(-1.5f, 1.f, 0.f);
                    int i = int(tar.x + 0.5f);
                    int j = int(tar.y + 0.5f);

                    if (i >= 0.f and i < 9.0f and
                        j >= 0.f and j < 4.5f) { // left side -> create match
                        newMatch();
                        phase_ = MAIN;
                    } else if (i >= 0.f and i < 9.0f and
                               j >= 4.5f and
                               j < 9.f) { // right side -> join match
                        joinMatch();
                        phase_ = MAIN;
                    }
                }
            }
        }


        // CURSOR
        glm::vec2 p = pointingAt(-1.f, 1.f, 0.f);
        // cursor pointing at table
        if (std::pow(p.x - 4.5f, 2) + std::pow(p.y - 4.5f, 2) < 55) {
            proto_cursor.get()->setPosition(
                    glm::vec3(p.x - 2.f, -1.f, p.y + 0.5f));
        } else {
            proto_cursor.get()->setPosition(glm::vec3(5.f, -20.f, 5.f));
        }


        // RENDER
        render();
        last_time_ = current_time_;
    }
}

void Game::mainLoop() {
    if (phase_ != MAIN) {
        return;
    }

    // MAIN LOOP
    std::shared_ptr<Renderable> proto_cursor =
            graphics_->createRenderable(data_folder + "flat_hor.obj",
                                        data_folder + "hand.png");


    //  debug scenery
    auto q = graphics_->createRenderable(data_folder + "stone.obj",
                                         data_folder + "red.png");
    q.get()->setPosition(glm::vec3(-6.f, -12.7f, -5.8f));
    std::shared_ptr<Renderable> qq =
            graphics_->createRenderable(data_folder + "stone.obj",
                                        data_folder + "black.png");
    qq.get()->setPosition(glm::vec3(2.f, 0.1f, -1.1f));
    std::shared_ptr<Renderable> qqq =
            graphics_->createRenderable(data_folder + "stone.obj",
                                        data_folder + "putting.png");
    qqq.get()->setPosition(glm::vec3(7.1f, 0.1f, -1.0f));
    std::shared_ptr<Renderable> qqqq =
            graphics_->createRenderable(data_folder + "stone.obj",
                                        data_folder + "texture.png");
    qqqq.get()->setPosition(glm::vec3(8.1f, 0.1f, -1.2f));

    std::shared_ptr<Renderable> table =
            graphics_->createRenderable(data_folder + "table.obj",
                                        data_folder + "table.png");

    std::shared_ptr<Renderable> scenery =
            graphics_->createRenderableLight(data_folder + "scenery.obj",
                                             data_folder + "scenery.png");


    // init board vector
    unsigned int size = board_->size();
    for (unsigned int i = 0; i < size; i++) {
        for (unsigned int j = 0; j < size; j++) {
            bb.push_back(
                    graphics_->createRenderable(
                            data_folder +
                            "stone.obj",
                            data_folder +
                            "texture.png"));

            bb[i * size + j].get()->setPosition(
                    glm::vec3(1.f * i,
                              0.f,
                              1.f * j));
        }
    }

/*
    // TODO: remove movement
    //TODO: addPosition to camera / create char class?
    bool deltaForwards = false;
    bool deltaBackwards = false;
    bool deltaLeft = false;
    bool deltaRight = false;
*/
    // board_->updateBoardThreaded();

    last_time_ = SDL_GetTicks();
    float last_time_board = SDL_GetTicks();


//    bool moving = false;
    while (phase_ == MAIN) {
        current_time_ = SDL_GetTicks();

        // INPUT
        while (SDL_PollEvent(&event_)) {
            if (event_.type == SDL_QUIT) {
                board_->forfeit();
                phase_ = BREAK;
            } else if (event_.type == SDL_MOUSEMOTION) {
                graphics_->getCamera()->addRotation(event_.motion.yrel,
                                                    event_.motion.xrel);

            } else if (event_.type == SDL_KEYDOWN) {

                // POHYB
                /*if (event_.key.keysym.sym == SDLK_w) {
                    deltaForwards = true;
                } else if (event_.key.keysym.sym == SDLK_s) {
                    deltaBackwards = true;
                } else if (event_.key.keysym.sym == SDLK_a) {
                    deltaLeft = true;
                } else if (event_.key.keysym.sym == SDLK_d) {
                    deltaRight = true;
                } else if (event_.key.keysym.sym == SDLK_l) {
                    moving = true;


                } else */if (event_.key.keysym.sym == SDLK_SPACE) {
                    board_->passTurn();
                } else if (event_.key.keysym.sym == SDLK_p) {
                    board_->forfeit();
                    phase_ = FINAL;
                }
            } /*else if (event_.type == SDL_KEYUP) {


                // TODO: remove movement
                // POHYB
                if (event_.key.keysym.sym == SDLK_w) {
                    deltaForwards = false;
                } else if (event_.key.keysym.sym == SDLK_s) {
                    deltaBackwards = false;
                } else if (event_.key.keysym.sym == SDLK_a) {
                    deltaLeft = false;
                } else if (event_.key.keysym.sym == SDLK_d) {
                    deltaRight = false;
                }

            } */else if (event_.type == SDL_MOUSEBUTTONDOWN) {
                if (event_.button.button == SDL_BUTTON_LEFT) {
                    // PUT STONE
                    if (board_->onTurn()) {
                        glm::vec2 tar = pointingAt(0.0f, 1.f, 0.f);
                        int i = int(tar.x + 0.5f);
                        int j = int(tar.y + 0.5f);
                        if (i >= 0 and i < board_->size() and
                            j >= 0 and j < board_->size()) {
                            bb[i * board_->size() + j]
                                    = graphics_->createRenderable(data_folder +
                                                                  "stone.obj",
                                                                  data_folder +
                                                                  "putting.png");
                            bb[i * board_->size() + j].get()->setPosition(
                                    glm::vec3(1.f * i, 0.f, 1.f * j));
                        }
                        board_->putStone(i, j);
                    }
                }
            }
        }

/*
        if (moving) {

            // POHYB
            const float movSpeed = 0.2f;
            graphics_->getCamera()->addPosition(glm::vec3(0.0f,
                                                   movSpeed * deltaRight -
                                                   movSpeed * deltaLeft,
                                                   movSpeed * deltaForwards -
                                                   movSpeed * deltaBackwards));
        }*/
        // TODO KURZOR
        /*
        proto_cursor.get()->setPosition(
                glm::vec3(graphics_->getCamera()->getFront() * 10.f +
                          graphics_->getCamera()->getPosition()));
        */

        // TODO rozhodnuti!

        if (board_->onTurn()) {
            glm::vec2 p = pointingAt(0.21f, 1.f, 0.f);

            if (p.x > -1.f && p.x < board_->size() + 0.f
                && p.y > -1.5f && p.y < board_->size() + 0.f) { // v poli
                proto_cursor.get()->setPosition(
                        glm::vec3(p.x - 1.f, 0.21f, p.y + 1.f));

            } else {
                proto_cursor.get()->setPosition(glm::vec3(0.f, -1.f, 0.f));
            }

        } else {

            proto_cursor.get()->setPosition(
                    glm::vec3(0.f, -1.f, 0.f));
        }

        // FPS
        render();
        last_time_ = current_time_;


        // BOARD UPDATE
        const int board_update_freq = 1500;
        if (current_time_ - last_time_board > board_update_freq) {
            SDL_Log("%i", board_->outgoingRequests());
            if (!board_->onTurn()) { // TODO + board_->outgoingRequests() == 0 ?
                std::thread t([&]() {
                    board_->updateOnTurn();
                    if (board_->should_i_update()) {
                        SDL_Log("UPDATING BOARD");
                        board_->updateWinner();
                        if (board_->getWinner() != '0') {
                            SDL_Log("WINNER == %c", board_->getWinner());
                            //TODO poradne
                            phase_ = FINAL;
                        }

                        // board update
                        std::thread t([&]() {
                            updateBoard();
/*

                            // NET


                            board_->updateBoard();

                            std::vector<std::vector<char>> board = board_->getBoard();

                            if (phase_ == MAIN) {

                                // RENDERABLES
                                creatingRenderable = true;
                                while (rendering) {
                                    SDL_Delay(1);
                                }

                                bb.clear();
                                const unsigned int size = board_->size();
                                for (unsigned int i = 0; i < size; i++) {
                                    for (unsigned int j = 0; j < size; j++) {
                                        if (board[i][j] == '1') {
                                            bb.push_back(
                                                    graphics_->createRenderable(
                                                            data_folder +
                                                            "stone.obj",
                                                            data_folder +
                                                            "black.png"));
                                        } else if (board[i][j] == '2') {
                                            bb.push_back(
                                                    graphics_->createRenderable(
                                                            data_folder +
                                                            "stone.obj",
                                                            data_folder +
                                                            "red.png"));
                                        } else {
                                            bb.push_back(
                                                    graphics_->createRenderable(
                                                            data_folder +
                                                            "stone.obj",
                                                            data_folder +
                                                            "texture.png"));
                                        }

                                        bb[i * size + j].get()->setPosition(
                                                glm::vec3(1.f * i,
                                                          0.f,
                                                          1.f * j));
                                    }
                                }
                                creatingRenderable = false;
                            }

 */
                        });
                        t.detach();
                    }
                });
                t.detach();

            }

            last_time_board = current_time_;
        }
    }

    bb.clear();
}


void Game::finalLoop() {
    if (phase_ != FINAL) {
        return;
    }

    board_->updateWinner();

    std::shared_ptr<Renderable> table =
            graphics_->createRenderable(data_folder + "table_empty.obj",
                                        data_folder + "table_empty.png");

    // game result on table
    std::shared_ptr<Renderable> on_table;
    if (board_->localWin()) {
        on_table =
                graphics_->createRenderable(data_folder + "on_table.obj",
                                            data_folder + "on_table_win.png");
    } else if (board_->getWinner() != '0') {
        on_table =
                graphics_->createRenderable(data_folder + "on_table.obj",
                                            data_folder + "on_table_lose.png");
    } else {
        on_table =
                graphics_->createRenderable(data_folder + "on_table.obj",
                                            data_folder + "TODO.png");
    }

    last_time_ = SDL_GetTicks();

    while (phase_ == FINAL) {
        current_time_ = SDL_GetTicks();

        // INPUT
        while (SDL_PollEvent(&event_)) {
            if (event_.type == SDL_KEYDOWN) {
            } else if (event_.type == SDL_QUIT) {
                phase_ = BREAK;
            } else if (event_.type == SDL_MOUSEMOTION) {
                graphics_->getCamera()->addRotation(event_.motion.yrel,
                                                    event_.motion.xrel);
            }
        }

        // RENDER
        render();
        last_time_ = current_time_;

    }

    if (board_ != nullptr) {
        SDL_Log("winner: %c", board_->getWinner());
    }

    SDL_Log("game ended");
}


glm::vec2 Game::pointingAt(float height, float size, float offset) {
    glm::vec3 front = graphics_->getCamera()->getFront();
    glm::vec3 pos = graphics_->getCamera()->getPosition();

    glm::vec3 look = pos + ((height - pos.y) / front.y) * front;
    float i = look.x / size + offset;
    float j = look.z / size + offset;

    return glm::vec2(i, j);
}




void Game::newMatch() {
    board_->createMatch();
    SDL_SetClipboardText(board_->getMatchID().c_str());
    SDL_Log("match created\nmatch id: %s", board_->getMatchID().c_str());
}

void Game::joinMatch() {
    std::string clipboard(SDL_GetClipboardText());
    board_->joinMatch(clipboard);
    SDL_Log("match joined");
}


void Game::render() {
    const int desired_fps_ms = 60;
    while (SDL_GetTicks() - last_time_ < (1000 / desired_fps_ms)) {
        graphics_->render();
    }
}


void Game::updateBoard() {
    // update board vector
    board_->updateBoard();
    std::vector<std::vector<char>> board = board_->getBoard();

    // create renderables
    if (phase_ == MAIN) {
        rendering_.lock();

        const unsigned int size = board_->size();
        for (unsigned int i = 0; i < size; i++) {
            for (unsigned int j = 0; j < size; j++) {
                if (board[i][j] == '1') {
                    bb[i * size + j] =
                            graphics_->createRenderable(
                                    data_folder + "stone.obj",
                                    data_folder + "black.png");
                } else if (board[i][j] == '2') {
                    bb[i * size + j] =
                            graphics_->createRenderable(
                                    data_folder + "stone.obj",
                                    data_folder + "red.png");
                } else {
                    bb[i * size + j] =
                            graphics_->createRenderable(
                                    data_folder + "stone.obj",
                                    data_folder + "texture.png");
                }

                bb[i * size + j].get()->setPosition(
                        glm::vec3(1.f * i,
                                  0.f,
                                  1.f * j));
            }
        }
        rendering_.unlock();
    }

}



