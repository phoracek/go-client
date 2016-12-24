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
	// TODO: predelat na tridu
	vec3 light_position = vec3(10.0f, 15.0f, 0.0f);
	// vec3 light_position_ = vec3(0.0f, -15.0f, 0.0f);
	vec3 light_intensities = vec3(0.85f, 0.85f, 0.85f);


	// diffuse
	mat3 normal_mat = transpose(inverse(mat3(model)));
	vec3 normal = normalize(normal_mat * normal_);

	vec3 frag_position = vec3(model * vec4(position_, 1));
	vec3 distance = light_position - frag_position;

	float brightness = dot(normal, distance) / (length(distance) * length(normal));
	brightness = clamp(brightness, 0, 1);


/*
	vec3 distance_ = light_position - frag_position;
	float brightness_ = dot(normal, distance_) / (length(distance_) * length(normal));
	brightness_ = clamp(brightness_, 0, 1);
	brightness += brightness_;
*/


	vec4 tex = texture(sampler, tex_coord_);
	if(tex.a == 0.0)
		discard;

	vec3 ambient = vec3(0.15f, 0.15f, 0.15f);
	vec3 diffuse = brightness * light_intensities;
	vec3 final_brightness = clamp(ambient + diffuse, 0, 1);

	color = vec4(vec4(final_brightness, 1) * tex.rgba);
}