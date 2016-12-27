#include "graphics.h"

#include <glm/gtc/type_ptr.hpp>

Graphics::Graphics(unsigned int width, unsigned int height) {
    SDL_Init(SDL_INIT_VIDEO);

    // SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    // SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);


    window_ = SDL_CreateWindow("",
                               SDL_WINDOWPOS_UNDEFINED,
                               SDL_WINDOWPOS_UNDEFINED,
                               width, height, SDL_WINDOW_OPENGL);

    context_ = SDL_GL_CreateContext(window_);

    glewExperimental = GL_TRUE;
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        SDL_LogCritical(SDL_LOG_CATEGORY_VIDEO, "opengl error");
        // SDL_Log();
    }
    GLenum glewinit = glewInit();
    if (glewinit != GLEW_OK) {
        SDL_LogCritical(SDL_LOG_CATEGORY_VIDEO, "glew init error");
        exit(-1);
    }

    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_CULL_FACE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);


    shader_program_ = new ShaderProgram("../data/vertex_shadow.glsl",
                                        "../data/fragment_shadow.glsl");
    shader_program_light_ = new ShaderProgram("../data/vertex_light.glsl",
                                              "../data/fragment_light.glsl");

    camera_ = new Camera;
    projection_ = glm::perspective(20.f, (GLfloat) width / (GLfloat) height,
                                   0.1f, 5000.0f);
}

Graphics::~Graphics() {
    delete shader_program_;
    delete camera_;

    SDL_GL_DeleteContext(context_);
    SDL_DestroyWindow(window_);
}

void Graphics::render() {
    // TODO better place for mutex?
    // std::lock_guard<std::mutex> guard(rendering_);

    rendering_.lock();

    // cleanup
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // prepare shader
    shader_program_->use();

    GLint model_loc = shader_program_->getUniform("model");
    GLint view_loc = shader_program_->getUniform("view");
    GLint proj_loc = shader_program_->getUniform("projection");
    glUniformMatrix4fv(view_loc, 1, GL_FALSE,
                       glm::value_ptr(camera_->getViewMatrix()));
    glUniformMatrix4fv(proj_loc, 1, GL_FALSE, glm::value_ptr(projection_));

    // NORMAL
    for (auto i: renderables_) {
        if (!i.expired()) {
            // set model matrix
            glm::vec3 pos = i.lock().get()->getPosition();
            glm::mat4 model = glm::mat4();
            model = glm::translate(model, pos);
            glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));
            // bind
            glBindTexture(GL_TEXTURE_2D,
                          i.lock().get()->getTexture().get()->getID());
            glBindVertexArray(i.lock().get()->getVAO().get()->getID());
            glDrawArrays(GL_TRIANGLES, 0,
                         i.lock().get()->getVAO().get()->getSize());
        }
    }

    // LIGHT
    shader_program_light_->use();

    glUniformMatrix4fv(view_loc, 1, GL_FALSE,
                       glm::value_ptr(camera_->getViewMatrix()));
    glUniformMatrix4fv(proj_loc, 1, GL_FALSE, glm::value_ptr(projection_));

    for (auto i: renderables_light_) {
        if (!i.expired()) {
            // set model matrix
            glm::vec3 pos = i.lock().get()->getPosition();
            glm::mat4 model = glm::mat4();
            model = glm::translate(model, pos);
            glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));
            // bind
            glBindTexture(GL_TEXTURE_2D,
                          i.lock().get()->getTexture().get()->getID());
            glBindVertexArray(i.lock().get()->getVAO().get()->getID());
            glDrawArrays(GL_TRIANGLES, 0,
                         i.lock().get()->getVAO().get()->getSize());
        }
    }

    glBindVertexArray(0);
    glUseProgram(0);

    SDL_GL_SwapWindow(window_);

    rendering_.unlock();
}


std::shared_ptr<Renderable> Graphics::createRenderable(std::string object_path,
                                                       std::string texture_path) {
    // TODO better place for mutex?
    // std::lock_guard<std::mutex> guard(rendering_);
    rendering_.lock();


    //TODO queue??????????????????
    // shader_program_->use(); //TODO ???

    // vao creation
    std::shared_ptr<VAO> temp_vao;
    if (object_list_[object_path].expired()) {
        temp_vao = std::shared_ptr<VAO>(new VAO(object_path));
        object_list_[object_path] = temp_vao;
    }

    // texture creation
    std::shared_ptr<Texture> temp_texture;
    if (texture_list_[texture_path].expired()) {
        temp_texture = std::shared_ptr<Texture>(
                new Texture(texture_path.c_str()));
        texture_list_[texture_path] = temp_texture;
    }

    // renderable creation
    std::shared_ptr<Renderable> temp = std::shared_ptr<Renderable>(
            new Renderable(object_list_[object_path].lock(),
                           texture_list_[texture_path].lock()));
    renderables_.push_back(temp);

    rendering_.unlock();

    return temp;
}

// TODO do jedne funkce
std::shared_ptr<Renderable>
Graphics::createRenderableLight(std::string object_path,
                                std::string texture_path) {
    // TODO better place for mutex?
    // std::lock_guard<std::mutex> guard(rendering_);
    rendering_.lock();

    //TODO queue??????????????????
    shader_program_light_->use(); //TODO ???

    // vao creation
    std::shared_ptr<VAO> temp_vao;
    if (object_list_[object_path].expired()) {
        temp_vao = std::shared_ptr<VAO>(new VAO(object_path));
        object_list_[object_path] = temp_vao;
    }

    // texture creation
    std::shared_ptr<Texture> temp_texture;
    if (texture_list_[texture_path].expired()) {
        temp_texture = std::shared_ptr<Texture>(
                new Texture(texture_path.c_str()));
        texture_list_[texture_path] = temp_texture;
    }

    // renderable creation
    std::shared_ptr<Renderable> temp = std::shared_ptr<Renderable>(
            new Renderable(object_list_[object_path].lock(),
                           texture_list_[texture_path].lock()));
    renderables_light_.push_back(temp);

    rendering_.unlock();

    return temp;
}

Camera *Graphics::getCamera() const {
    return camera_;
}