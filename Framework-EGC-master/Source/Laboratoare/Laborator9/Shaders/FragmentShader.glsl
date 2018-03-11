#version 330
 
uniform sampler2D texture_1;
uniform sampler2D texture_2;
 
in vec2 texcoord;

layout(location = 0) out vec4 out_color;

void main()
{
	// TODO : calculate the out_color using the texture2D() function

	vec4 color1 = texture2D(texture_1, texcoord);
	vec4 color2 = texture2D(texture_2, texcoord);

	if(color1.a < 0.5f) {
		color1 = vec4(0);
	}

	if(color2.a < 0.5f) {
		color2 = vec4(0);
	}

	vec3 color = mix(color1.xyz, color2.xyz, 0.5f);
	out_color = vec4(color, 1);
}