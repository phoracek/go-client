#include "renderable.h"

Renderable::Renderable(std::shared_ptr<VAO> vao,
                       std::shared_ptr<Texture> texture) :
        vao_(vao), texture_(texture),
        position_(glm::vec3(0.0f, 0.0f, 0.0f)) {
}

Renderable::~Renderable() {
}

std::shared_ptr<VAO> Renderable::getVAO() const {
    return vao_;
}

std::shared_ptr<Texture> Renderable::getTexture() const {
    return texture_;
}

void Renderable::setPosition(glm::vec3 position) {
    position_ = position;
}

glm::vec3 Renderable::getPosition() const {
    return position_;
}