#version 330

// TODO: get color value from vertex shader
in vec3 world_position;
in vec3 world_normal;
in vec2 texture_coord;

// Uniforms for light properties
uniform vec3 light_color;
uniform vec3 light_position;
uniform vec3 eye_position;
uniform vec3 color;


uniform int material_shininess;
uniform int no_layers;
uniform int toon;

uniform sampler2D texture_1;
uniform sampler2D texture_2;

layout(location = 0) out vec4 out_color;

void main()
{
	// TODO: compute world space vectors
	vec3 L = normalize(light_position - world_position);
	vec3 V = normalize(eye_position - world_position);
	vec3 H = normalize(L + V);
	vec3 R = reflect(L,world_normal);
	
	// TODO: define ambient light component
	float ambient_light = 0.25;

	// TODO: compute diffuse light component
	float diffuse_light = max(ambient_light, dot(world_normal,L));

	// TODO: compute specular light component
	float specular_light = 0;

	if (diffuse_light > 0)
	{
		specular_light =  pow(max(0, dot(world_normal,H)), material_shininess);
	}

	// TODO: compute light
	float Kc = 1, Kl = 0.0002, Kq = 0.0005;
	float d = length(world_position - light_position);
	float attenL = 1/(Kc + d* Kl + pow(d, 2) * Kq);

	vec4 color1 = texture2D(texture_1, texture_coord);
	vec4 color2 = texture2D(texture_2, texture_coord);
	vec3 material_kd = color1.xyz;
	vec3 material_ks = color2.xyz;

	if (toon == 1) 
	{
		diffuse_light = floor(diffuse_light * no_layers) / no_layers;
		specular_light = floor(specular_light * no_layers) / no_layers;
	} 
	
	if (no_layers == 0) {
		material_kd = vec3(0.5);
		material_ks = vec3(0.5);
	}

	vec3 final_color = ((ambient_light + diffuse_light) * material_kd + specular_light * material_ks) * light_color * attenL; 
	
	out_color = vec4(final_color, 1);
}