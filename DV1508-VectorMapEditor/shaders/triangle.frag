#version 440 core

out vec4 outColor;

in vec2 vUV;

uniform sampler2D tex;

void main()
{
	outColor = vec4(texture(tex, vUV).rgba);
}