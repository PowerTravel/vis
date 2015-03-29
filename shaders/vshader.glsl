#version  330 core
layout (location = 0) in vec3 vPosition;
//layout (location = 1) in vec2 tempTex;
//layout (location = 2) in vec3 vNormals;
uniform mat4 M, V, P;

void main(){
	mat4 MET = M;
	mat4 VET = V;
	mat4 PET = P;
	gl_Position = P*V*M*vec4(vPosition,1);
}

