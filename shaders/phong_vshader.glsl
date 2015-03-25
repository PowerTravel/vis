#version  330 core
layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec2 tempTex;
layout (location = 2) in vec3 vNormals;
uniform mat4 M, V, P;
uniform mat4 B,Pl,Vl,Ml;
varying vec2 texCoord0; 
uniform int nrLights;

uniform vec3 lightPosition[3];
varying vec3 L[3],E[3],H[3],N[3];
varying float R[3];
varying vec4 ShadowCoord;

varying vec4 pos2;

void main()
{
	texCoord0 = tempTex;
	mat4 VM = V*M;
	vec4 vp = vec4(vPosition,1);
	vec3 pos = (VM * vp).xyz;
	vec4 vn = vec4(vNormals, 0.0);

	for(int i = 0; i<nrLights; i++)
	{
		vec4 lp = vec4(lightPosition[i],1);
		R[i] = length( (V*lp).xyz - pos);
		L[i] = normalize( (V*lp).xyz - pos);
		E[i] = normalize(-pos);
		H[i] = normalize(L[i]+E[i]);
		N[i] = normalize(VM*vn).xyz;
	}

	ShadowCoord =B * Pl*Vl*Ml*vec4(vPosition, 1);

//	gl_Position = ShadowCoord;
	
	gl_Position = P*vec4(pos,1);
}
