#ifndef SHADER_PROGRAM_
#define SHADER_PROGRAM_


#include <GL/glew.h>


class ShaderProgram {
public:
    ShaderProgram(const char* vertex_path, const char* fragment_path);
    ~ShaderProgram();

    void use() const;

    GLint getAttrib(const GLchar* attrib_name) const;
    GLint getUniform(const GLchar* uniform_name) const;

private:
    GLuint id_;
};


#endif // SHADER_PROGRAM_