#ifndef RENDERABLE_H_
#define RENDERABLE_H_


#include "vao.h"
#include "texture.h"

#include <memory>

#include <GL/glew.h>
#include <glm/glm.hpp>


class Renderable {
public:
    Renderable(std::shared_ptr<VAO> vao, std::shared_ptr<Texture> texture);
    ~Renderable();

    void setPosition(glm::vec3 position);

    glm::vec3 getPosition() const;

    std::shared_ptr<VAO> getVAO() const;
    std::shared_ptr<Texture> getTexture() const;

private:
    std::shared_ptr<VAO> vao_;
    std::shared_ptr<Texture> texture_;

    glm::vec3 position_;
};


#endif // RENDERABLE_H_