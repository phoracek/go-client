#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#include <vector>
#include <string>
#include <map>
#include <memory>
#include <mutex>
#include <thread>
#include <chrono>

#include <SDL.h>
#include <SDL2/SDL_image.h>
#include <glm/glm.hpp>

#include "shader_program.h"
#include "renderable.h"
#include "vao.h"
#include "texture.h"
#include "camera.h"

class Graphics {
public:
    Graphics(unsigned int width, unsigned int height);
    ~Graphics();

    void render();

    std::shared_ptr<Renderable> createRenderable(std::string object_path,
                                                 std::string texture_path);
    std::shared_ptr<Renderable> createRenderableLight(std::string object_path,
                                                 std::string texture_path);

    Camera *getCamera();

private:
    SDL_Window *window_;
    SDL_GLContext context_;

    ShaderProgram *shader_program_;
    ShaderProgram *shader_program_light_;

    glm::mat4 projection_;
    Camera *camera_;

    std::vector<std::weak_ptr<Renderable>> renderables_;
    std::vector<std::weak_ptr<Renderable>> renderables_light_;
    std::map<std::string, std::weak_ptr<VAO>> object_list_;
    std::map<std::string, std::weak_ptr<Texture>> texture_list_;
	
	std::mutex rendering_;
};

#endif // GRAPHICS_H_
