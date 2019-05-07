#version 440 core

out vec4 outColor;

in vec3 vPos;

void main()
{
	vec3 dx = dFdx(vPos);
	vec3 dy = dFdy(vPos);
	vec3 normal = normalize(cross(dx, dy));

	vec3 matColor = vec3(1,0,0);

	const vec3 light = vec3(10,10,-10);


	vec3 l = normalize(light - vPos);

	vec3 lighting = vec3(0);
	lighting += matColor * vec3(0.2);
	lighting += max(dot(l, normal), 0) * vec3(0.8);
	
	vec3 color = lighting;

	outColor = vec4(color,1);
}