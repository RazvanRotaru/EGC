#version 330

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

uniform int outline;
uniform int time;

// Output values to fragment shader
out vec3 world_position;
out vec3 world_normal;
out vec2 texture_coord;

void main()
{
	// TODO: compute world space vertex position and normal
	// TODO: send world position and world normal to Fragment Shader
	world_position = (Model * vec4(v_position, 1)).xyz;
	world_normal = normalize(mat3(Model) * v_normal);
	float x = v_texture_coord.x;
	float y = v_texture_coord.y;
	texture_coord = vec2(x, y);

	gl_Position = Projection * View * Model * vec4(v_position, 1.0);
}
