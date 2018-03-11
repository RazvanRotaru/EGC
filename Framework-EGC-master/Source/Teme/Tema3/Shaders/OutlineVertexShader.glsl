#version 330

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

uniform float puls; 
uniform int outline;

// Output values to fragment shader

void main()
{
	vec3 new_position = v_position;
	if (outline == 1) 
	{
		new_position += v_normal * puls;
	}
	gl_Position = Projection * View * Model * vec4(new_position, 1.0);
}
