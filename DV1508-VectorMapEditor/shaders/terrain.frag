#version 440 core

layout(location = 0) out vec4 outColor;

in vec3 vPos;
in vec2 vUV;

uniform vec3 camPos;
uniform vec2 brushUV;
uniform float brushRadius;

void brush(inout vec3 color)
{
	
	float radius = brushRadius;
	float thickness = 0.005;
	float len = length(vUV - brushUV);
	float t = smoothstep(radius, radius - thickness, len)*smoothstep(radius - thickness, radius, len);
	color = mix(color, vec3(3,0,0), t);
}

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

	brush(color);

	outColor = vec4(color,1);
}