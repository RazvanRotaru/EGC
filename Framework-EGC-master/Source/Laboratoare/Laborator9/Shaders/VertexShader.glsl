#version 330

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;
layout(location = 3) in vec3 v_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

uniform float time;
uniform int useTime;

out vec2 texcoord;

void main()
{
	// TODO : pass v_texture_coord as output to Fragment Shader
	float y = v_texture_coord.y;
	float x = v_texture_coord.x;
	if (useTime == 1) {
		// y = y + time / 10;
		x = x - time ;
	} 

	texcoord = vec2(x, y);

	gl_Position = Projection * View * Model * vec4(v_position, 1.0);
}
