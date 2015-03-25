#version 330 core

uniform mat4 M,V,P;
layout (location = 0) in vec3 vPosition;

void main(){
	gl_Position = P*V*M*vec4(vPosition, 1);
}
