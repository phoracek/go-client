#include "texture.h"

#include <string>
#include <iostream>

#include <SDL2/SDL_image.h>

Texture::Texture(const char* path) {
    SDL_Surface* image = IMG_Load(path);

    // error checking
    if (!image) {
        std::string error_message =
                "Error while loading image (" + std::string(path) + ").";
        SDL_Log(error_message.c_str());
        return;
    }

    GLenum texture_format, internal_format, tex_type;
    if (image->format->BytesPerPixel == 4) {
        if (image->format->Rmask == 0x000000ff) {
            texture_format = GL_RGBA;
            tex_type = GL_UNSIGNED_INT_8_8_8_8_REV;
        } else {
            texture_format = GL_BGRA;
            tex_type = GL_UNSIGNED_INT_8_8_8_8;
        }
        internal_format = GL_RGBA8;
    } else {
        if (image->format->Rmask == 0x000000ff) {
            texture_format = GL_RGB;
            tex_type = GL_UNSIGNED_BYTE;
        } else {
            texture_format = GL_BGR;
            tex_type = GL_UNSIGNED_BYTE;
        }
        internal_format = GL_RGB8;
    }

    /*
    GLenum texture_format = image->format->BytesPerPixel == 4 ? GL_RGBA : GL_RGB;
    GLenum internal_format =
            image->format->BytesPerPixel == 4 ? GL_RGBA : GL_RGB;

    glTexImage2D(GL_TEXTURE_2D, 0, internal_format, image->w, image->h, 0,
                 texture_format,
                 GL_UNSIGNED_BYTE, image->pixels);
    */


    glGenTextures(1, &id_);
    glBindTexture(GL_TEXTURE_2D, id_);

    glTexImage2D(GL_TEXTURE_2D, 0, internal_format, image->w, image->h, 0,
                 texture_format,
                 tex_type, image->pixels);


    // glUniform1i(shader_program_->getUniform("texture"), 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    // glGenerateMipmap(GL_TEXTURE_2D);


    // cleanup
    SDL_FreeSurface(image);
}

Texture::~Texture() {
    glDeleteTextures(1, &id_);
}

GLuint Texture::getID() const {
    return id_;
}