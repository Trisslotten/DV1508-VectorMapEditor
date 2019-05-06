#version 440 core

in vec2 uv;

out vec3 vPos;

uniform mat4 camTransform;

// https://gist.github.com/patriciogonzalezvivo/670c22f3966e662d2f83
float rand(vec2 n) { 
	return fract(sin(dot(n, vec2(12.9898, 4.1414))) * 43758.5453);
}
float noise(vec2 p){
	vec2 ip = floor(p);
	vec2 u = fract(p);
	u = u*u*(3.0-2.0*u);
	
	float res = mix(
		mix(rand(ip),rand(ip+vec2(1.0,0.0)),u.x),
		mix(rand(ip+vec2(0.0,1.0)),rand(ip+vec2(1.0,1.0)),u.x),u.y);
	return res*res;
}

void main()
{
	vec2 t = 10*(2*uv-1);
	vec3 pos = vec3(t.x, 0.0, t.y);
	float a = 2;
	float f = 5;
	for(int i = 0; i < 10; i++) 
	{
		pos.y += a*noise(f*uv);
		a *= 0.4;
		f /= 0.5;
	}

	vPos = pos;
	gl_Position = camTransform * vec4(pos, 1.0);
}