#version 330

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

// Uniforms for light properties
uniform vec3 light_position;
uniform vec3 eye_position;
uniform vec3 material_kd;
uniform vec3 material_ks;
uniform int material_shininess;

// Output value to fragment shader
out vec3 color;

void main()
{
	// TODO: compute world space vectors
	vec3 world_pos = (Model * vec4(v_position, 1)).xyz;
	vec3 N = normalize(mat3(Model) * v_normal);

	vec3 L = normalize(light_position - world_pos);
	vec3 V = normalize(eye_position - world_pos);
	vec3 H = normalize(L + V);
	vec3 R = reflect(L,N);

	// TODO: define ambient light component
	float ambient_light = 0.25;

	// TODO: compute diffuse light component
	float diffuse_light = max(ambient_light, dot(N,L));

	// TODO: compute specular light component
	int recvLight = dot(L,N) > 0 ? 1 : 0;
	float specular_light;

	if (diffuse_light > 0)
	{
	specular_light =  pow(max(0, dot(N,H)), material_shininess);
	}

	// TODO: compute light
	float d = length(world_pos - light_position);
	float attenL = 1/(1 + d * 0.0002 + pow(d,2) * 0.0005);

	vec3 OC = ambient_light + (diffuse_light * material_kd + specular_light * material_ks) * attenL;
	// TODO: send color light output to fragment shader
	color = OC;

	gl_Position = Projection * View * Model * vec4(v_position, 1.0);
}
