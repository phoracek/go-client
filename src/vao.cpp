#include "vao.h"

#include <glm/glm.hpp>
#include <vector>
#include <fstream>
#include <iostream>

bool loadModel(std::string path, std::vector<glm::vec3> &out_vertices,
               std::vector<glm::vec2> &out_uvs,
               std::vector<glm::vec3> &out_normals) {
    // TODO: error checking
    std::vector<unsigned int> vertex_indices, uv_indices, normal_indices;
    std::vector<glm::vec3> temp_vertices;
    std::vector<glm::vec2> temp_uvs;
    std::vector<glm::vec3> temp_normals;

    std::ifstream files(path);
    std::string temp;

    while (files >> temp) {
        if (temp == "#")
            continue;
        else if (temp == "v") {
            glm::vec3 vertex;
            files >> vertex.x >> vertex.y >> vertex.z;
            temp_vertices.push_back(vertex);
        } else if (temp == "vt") {
            glm::vec2 uv;
            files >> uv.x >> uv.y;
            uv.y = -uv.y;
            temp_uvs.push_back(uv);
        } else if (temp == "vn") {
            glm::vec3 normal;
            files >> normal.x >> normal.y >> normal.z;
            temp_normals.push_back(normal);
        } else if (temp == "f") {
            char temp_ch;
            unsigned int vertex_index[3], uv_index[3], normal_index[3];
            for (unsigned int i = 0; i < 3; i++) {
                files >>
                      vertex_index[i] >> temp_ch >>
                      uv_index[i] >> temp_ch >>
                      normal_index[i];

                vertex_indices.push_back(vertex_index[i]);
                uv_indices.push_back(uv_index[i]);
                normal_indices.push_back(normal_index[i]);
            }
        }
    }

    for (unsigned int i = 0; i < vertex_indices.size(); i++) {
        out_vertices.push_back(temp_vertices[vertex_indices[i] - 1]);
        out_uvs.push_back(temp_uvs[uv_indices[i] - 1]);
        out_normals.push_back(temp_normals[normal_indices[i] - 1]);
    }

    return true;
}


VAO::VAO(std::string path) {
    std::vector<glm::vec3> verts;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> norms;
    loadModel(path.c_str(), verts, uvs, norms);

    GLuint VBO_ver, VBO_tex, VBO_nor;
    glGenVertexArrays(1, &id_);
    glGenBuffers(1, &VBO_ver);
    glGenBuffers(1, &VBO_tex);
    glGenBuffers(1, &VBO_nor);

    glBindVertexArray(id_);
    // vertices VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO_ver);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * verts.size(), &verts[0],
                 GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
    glEnableVertexAttribArray(0);
    // uv VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO_tex);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * uvs.size(), &uvs[0],
                 GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);
    glEnableVertexAttribArray(1);
    // normal VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO_nor);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * norms.size(),
                 &norms[0],
                 GL_STATIC_DRAW);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    size_ = GLuint(
            sizeof(glm::vec3) * verts.size() / sizeof(GLfloat)
            / 3.f);
}

VAO::~VAO() {
    std::cout << "delted " << id_ << std::endl;

    glDeleteVertexArrays(1, &id_);
}


GLuint VAO::getID() const {
    return id_;
}

GLuint VAO::getSize() const {
    return size_;
}