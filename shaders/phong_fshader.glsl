#version 330 core
out vec4 fragColor;

		uniform vec4 ambientProduct[3],diffuseProduct[3],specularProduct[3]; 
uniform float attenuation[3];
uniform float shininess;
uniform int usingDiffTexture, usingShadowMap, nrLights;
uniform sampler2D diffuseTextureID;//, shadowMapID;
//uniform sampler2DShadow shadowMapID;
uniform sampler2D shadowMapID;
varying vec3 L[3],E[3],H[3],N[3];
varying float R[3];
varying vec2 texCoord0; 
varying vec4 ShadowCoord;



varying vec4 pos2;

void main() 
{ 
	float visibility=1;
	vec4 color = vec4(0,0,0,0);
	for(int i = 0; i<nrLights; i++)
	{
		vec4 ambient, diffuse, specular;
		
		ambient = ambientProduct[i];

		float Kd = max(dot(L[i],N[i]), 0.0);
		if(usingDiffTexture != 0)
		{
			float alpha=1;
			diffuse = Kd*(alpha*texture2D(diffuseTextureID,texCoord0.st)+(1-alpha)*diffuseProduct[i]);
			//diffuse = (alpha*texture2D(diffuseTextureID,texCoord0.st)+(1-alpha)*diffuseProduct[i]);
		}else{
			diffuse = Kd*diffuseProduct[i];
		}

		float Ks = pow(max( dot(N[i],H[i]), 0.0 ), shininess);
		specular = Ks * specularProduct[i];

		if(dot(L[i],N[i]) < 0.0)
		{
			diffuse = vec4(0.0,0.0,0.0,1.0);
			specular = vec4(0.0, 0.0, 0.0, 1.0);
		}
	
		float att = 1/( 1 + attenuation[i]*pow(R[i],2) );
	
		visibility = 1;
		// We only support shadows for the first light
		if(usingShadowMap != 0 && i==0){
//			if(texture(shadowMapID, vec3(ShadowCoord.xy,1)) < ShadowCoord.z){	
//			if(texture2D(shadowMapID, ShadowCoord.xy).z < ShadowCoord.z){	
				//visibility=0.9;
//			}
		}
		color += ambient+visibility*att*(diffuse+specular);
	}

	fragColor = color;
	//fragColor = ShadowCoord;
	//fragColor = texture2D( shadowMapID, ShadowCoord.xy).xxxx;
	//fragColor = texture2D(shadowMapID, texCoord0.xy).xxxx;
	//fragColor = vec4(ShadowCoord.xy,0,1);	fragColor = vec4(texCoord0.st,0,1);
//	visibility = texture( shadowMapID, vec3(ShadowCoord.xy, (ShadowCoord.z)/ShadowCoord.w) );
}
