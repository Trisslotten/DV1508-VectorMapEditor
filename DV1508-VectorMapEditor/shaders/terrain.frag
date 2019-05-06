#version 440 core

out vec4 outColor;

in vec3 vPos;

void main()
{
	vec3 dx = dFdx(vPos);
	vec3 dy = dFdy(vPos);
	vec3 normal = normalize(cross(dx, dy));

	vec3 color = normal;
	outColor = vec4(color,1);
}