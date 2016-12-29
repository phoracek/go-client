#version 130

uniform sampler2D sampler;
in vec2 tex_coord_;

out vec4 color;

void main()
{
    color = texture(sampler, tex_coord_);
}
