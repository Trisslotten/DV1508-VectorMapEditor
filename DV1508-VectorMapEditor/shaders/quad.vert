#version 440 core

in vec2 pos;

out vec2 vUV;

uniform float aspect;
uniform vec2 scale;
uniform vec2 offset;

void main()
{
	vUV = (pos+1.0)/2.0;

	vec2 p = scale*pos;
	p.x /= aspect;
	p+=offset;

	gl_Position = vec4(p, 0, 1);
}