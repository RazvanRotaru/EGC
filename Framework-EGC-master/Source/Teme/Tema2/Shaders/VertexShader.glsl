#version 330

// TODO: get vertex attributes from each location

layout(location = 0) in vec3 v_position;
layout(location = 3) in vec3 v_normal;
layout(location = 2) in vec2 v_texture;
layout(location = 1) in vec3 v_color;

out vec3 frag_position;
out vec3 frag_normal;
out vec2 frag_texture;
out vec3 frag_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform float Time;

// TODO: output values to fragment shader

void main()
{
	// TODO: send output to fragment shader
	frag_position = v_position;
	frag_normal = v_normal;
	frag_texture = v_texture;
	frag_color = v_color;

	if (v_color == vec3(0.5)) 
		frag_color = vec3(0.65) - Time * 0.5 * v_color;
	if (v_color == vec3(-0.15,0.7,0)) 
		frag_color = vec3(0,0.9,0) - Time * 0.5 * v_color;
	if (v_color == vec3(0.5,1.2,0.9))
		frag_color = vec3(0.5,0.7,0.9) - Time * 0.5 * v_color;

	// TODO: compute gl_Position
	 gl_Position = Projection * View * Model * vec4(v_position, 1.0);
}
