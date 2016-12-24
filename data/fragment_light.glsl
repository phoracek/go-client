#version 330 core

precision mediump float;

uniform sampler2D sampler;
uniform mat4 model;

in vec3 position_;
in vec2 tex_coord_;
in vec3 normal_;

out vec4 color;

void main()
{
	vec4 tex = texture(sampler, tex_coord_);
	if(tex.a == 0.0)
		discard;

	color = vec4(tex.rgba);
}