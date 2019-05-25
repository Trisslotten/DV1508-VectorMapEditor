#version 440 core

in vec2 uv;

out vec2 vUV;

void main()
{
	vUV = uv;
	vec3 pos = vec3(0);
	pos.xy = 2*uv - 1;
	gl_Position = vec4(pos, 1);
}