#version 330

uniform mat4 matVP;
uniform mat4 matGeo;

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec4 col;

out vec4 color;

// Higher values are smaller width.
uniform float outline_width = 5.0;

vec4 base_color = vec4(1, 1, 1, 1);

float fresnel(float amount, vec3 normal, vec3 view)
{
   return pow((1.0 - clamp(dot(normalize(normal), normalize(view)), 0.0, 1.0 )), amount);
}

void main()
{
	float basic_fresnel = fresnel(outline_width, normal, pos);
	basic_fresnel = step(0.5, basic_fresnel); //0.5 magic number seems to give the most intuitive control
	color = col - (basic_fresnel / 4);
	gl_Position = matVP * matGeo * vec4(pos, 1);
}