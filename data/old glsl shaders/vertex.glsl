#version 130

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

in vec3 position;
in vec2 tex_coord;

out vec2 tex_coord_;

void main()
{
    tex_coord_ = tex_coord;
    gl_Position = projection * view * model * vec4(position, 1.0f);
}