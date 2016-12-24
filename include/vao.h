#ifndef VAO_H_
#define VAO_H_

#include <string>

#include <GL/glew.h>

class VAO {
public:
    VAO(std::string path);
    ~VAO();

    GLuint getID() const;
    GLuint getSize() const;

private:
    GLuint id_;
    GLuint size_;
};

#endif // VAO_H_