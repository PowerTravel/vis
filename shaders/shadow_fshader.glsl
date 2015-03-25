#version 330 core

layout(location=0) out float fragmentDepth;

void main(){

	fragmentDepth = gl_FragCoord.z;
//	gl_FragColor.r = gl_FragCoord.z; 
//	gl_FragColor.g = gl_FragCoord.z; 
//	gl_FragColor.b = gl_FragCoord.z; 
}
