#version  330 core
layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec2 tempTex;
layout (location = 2) in vec3 vNormals;
layout (location = 4) in vec3 vParticle;
uniform mat4 M, V, P;
uniform vec3 lPos;
out vec3 L,E,H,N;
out float r;

void main()
{
	mat4 VM = V*M;
	vec4 vp = vec4(vPosition+vParticle,1);
	vec3 pos = (VM * vp).xyz;
	vec4 vn = vec4(vNormals, 0.0);

	vec4 lp = vec4(lPos,1);
	r = length( (V*lp).xyz - pos);
	L = normalize( (V*lp).xyz - pos);
	E = normalize(-pos);
	H = normalize(L+E);
	N = normalize(VM*vn).xyz;


	vec3 vPart = (V * vec4(vParticle,1)).xyz;

	gl_Position = P*vec4(pos,1);
}
