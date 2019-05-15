#version 440 core

out vec4 outColor;

in vec3 vPos;

uniform vec3 camPos;

void main()
{
	vec3 dx = dFdx(vPos);
	vec3 dy = dFdy(vPos);
	vec3 normal = normalize(cross(dx, dy));

	vec3 matColor = vec3(44, 176, 55)/256;
	matColor = vec3(0.7);

	const vec3 lightDir = normalize(vec3(1));
	vec3 look = normalize(camPos - vPos);

	
	float diffuse = max(dot(lightDir, normal), 0);
	vec3 h = normalize(look + lightDir);
	float specular = pow(clamp(dot(normal, h), 0.0, 1.0), 500.0);

	vec3 lighting = vec3(0);
	lighting += vec3(0.2);
	lighting += diffuse * vec3(0.8);
	//lighting += specular * vec3(1.0);
	lighting *= matColor;

	vec3 color = lighting;

	outColor = vec4(color,1);
}