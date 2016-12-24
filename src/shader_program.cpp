#include "shader_program.h"
#include "file_loader.h"

#include <iostream>
#include <SDL_log.h>


ShaderProgram::ShaderProgram(const char* vertex_path,
                             const char* fragment_path) {
    // shader init
    // vertex shader
    GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
    std::string vert_code = FileLoader::load(vertex_path);
    const GLchar* vert_code_char = vert_code.c_str();
    const GLint vert_code_length = vert_code.size();
    glShaderSource(vertex, 1, &vert_code_char, &vert_code_length);
    glCompileShader(vertex);
    //fragment shader
    GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
    std::string frag_code = FileLoader::load(fragment_path);
    const GLchar* frag_code_char = frag_code.c_str();
    const GLint frag_code_length = frag_code.size();
    glShaderSource(fragment, 1, &frag_code_char, &frag_code_length);
    glCompileShader(fragment);
    // program
    id_ = glCreateProgram();
    glAttachShader(id_, vertex);
    glAttachShader(id_, fragment);
    glLinkProgram(id_);

    // error checking
    int buff_len = 0;
    glGetShaderiv(vertex, GL_INFO_LOG_LENGTH, &buff_len);
    if (buff_len > 1) {
        GLchar* log_string = new char[buff_len + 1];
        glGetShaderInfoLog(vertex, buff_len, 0, log_string);
    }
    buff_len = 0;
    glGetShaderiv(fragment, GL_INFO_LOG_LENGTH, &buff_len);
    if (buff_len > 1) {
        GLchar* log_string = new char[buff_len + 1];
        glGetShaderInfoLog(fragment, buff_len, 0, log_string);
        SDL_Log(log_string);
    }
    buff_len = 0;
    glGetProgramiv(id_, GL_INFO_LOG_LENGTH, &buff_len);
    if (buff_len > 1) {
        GLchar* log_string = new char[buff_len + 1];
        glGetProgramInfoLog(id_, buff_len, 0, log_string);
        SDL_Log(log_string);
    }

    // cleanup
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

ShaderProgram::~ShaderProgram() {
    glDeleteProgram(id_);
}

void ShaderProgram::use() const {
    glUseProgram(id_);
}

GLint ShaderProgram::getAttrib(const GLchar* attrib_name) const {
    return glGetAttribLocation(id_, attrib_name);
}

GLint ShaderProgram::getUniform(const GLchar* uniform_name) const {
    return glGetUniformLocation(id_, uniform_name);
}
