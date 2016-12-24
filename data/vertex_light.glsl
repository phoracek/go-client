#version 330 core

// precision mediump float;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 tex_coord;
layout(location = 2) in vec3 normal;

out vec3 position_;
out vec2 tex_coord_;
out vec3 normal_;

void main()
{
	position_ = position;
	tex_coord_ = tex_coord;
	normal_ = normal;
	
	gl_Position = projection * view * model * vec4(position, 1.0f);
}