#version 440 core

layout(location = 0) out vec4 outColor;
layout(location = 1) out vec4 outUV;

in vec3 vPos;
in vec2 vUV;

layout(std430, binding = 1) buffer buf
{
	vec2 brushUV;
};

uniform vec3 camPos;
uniform float brushRadius;
uniform float brushStrength;

uniform sampler2D vectorMap;

void brush(inout vec3 color)
{
	float dx = length(dFdx(vPos));
	float dy = length(dFdy(vPos));

	float radius = brushRadius;
	float thickness = 1.5*max(dx,dy);
	float len = length(vUV - brushUV);
	float t = smoothstep(radius, radius - thickness, len)*smoothstep(radius - thickness, radius, len);

	vec3 highColor = vec3(3,0,0);
	vec3 lowColor = vec3(0,0,3);
	vec3 circleColor = mix(lowColor, highColor, brushStrength);

	color = mix(color, circleColor, t);
}

vec3 calcNormal()
{
	vec2 size = textureSize(vectorMap, 0);
	vec2 t = vec2(1.0/size.x, 0);
	vec2 wt = 2.*t;
	vec3 p1 = texture(vectorMap, vUV).xyz;
	vec3 p2 = wt.xyy + texture(vectorMap, vUV + t.xy).xyz;
	vec3 p3 = wt.yyx + texture(vectorMap, vUV + t.yx).xyz;
	vec3 v1 = p2 - p1;
	vec3 v2 = p3 - p1;
	return normalize(cross(v2, v1));
}


void main()
{
	vec3 matColor = vec3(44, 176, 55)/256;
	matColor = vec3(0.5);

	vec3 color = matColor;

	brush(color);

	outColor = vec4(color,1);
	outUV = vec4(vUV, 0, 1);
}