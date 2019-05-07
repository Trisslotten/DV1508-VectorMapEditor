#version 440 core

in vec2 uv;

out vec3 vPos;

uniform mat4 camTransform;
uniform sampler2D vectorMap;

void main()
{
	vec2 t = 10*(2*uv-1);
	vec3 pos = vec3(t.x, 0.0, t.y);

	vec3 disp = texture(vectorMap, uv).xyz;
	pos += disp;

	vPos = pos;
	gl_Position = camTransform * vec4(pos, 1.0);
}