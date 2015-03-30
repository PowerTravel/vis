#version 330 core
out vec4 fragColor;

uniform vec4 Amb,Diff,Spec; 
uniform float shi, att;

in vec3 L,E,H,N;
in float r;

void main() 
{ 
	vec4 color = vec4(0,0,0,0);
	vec4 ambient, diffuse, specular;
		
	ambient = Amb;

	float Kd = max(dot(L,N), 0.0);
	diffuse = Kd*Diff;
	
	float Ks = pow(max( dot(N,H), 0.0 ), shi);
	specular = Ks * Spec;

	if(dot(L,N) < 0.0)
	{
		diffuse = vec4(0.0,0.0,0.0,1.0);
		specular = vec4(0.0, 0.0, 0.0, 1.0);
	}

	//float attenuation = 1/( 1 + att*pow(r,2) );
	float attenuation = 1;
	color += ambient+attenuation*(diffuse+specular);

	fragColor = color;
}
