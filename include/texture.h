#ifndef TEXTURE_H_
#define TEXTURE_H_


#include <GL/glew.h>


class Texture {
public:
    Texture(const char* path);
    ~Texture();

    GLuint getID() const;

private:
    GLuint id_;
};


#endif // TEXTURE_H_