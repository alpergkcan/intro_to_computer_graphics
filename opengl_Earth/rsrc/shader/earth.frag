#version 430 core

in vec3 Pos;
in vec3 Normal;
in vec2 TexCoord;

uniform vec3 lightPos; 
uniform vec3 cameraPos; 

uniform sampler2D TexColor;

out vec4 FragColor;

void main()
{
	// variables
	vec3 objColor      = texture(TexColor, TexCoord).xyz;

	vec3 ambientLight  = vec3(0.6, 0.6, 0.6);
	vec3 ambientRC     = vec3(0.5, 0.5, 0.5);

	vec3 specularLight = vec3(1.0, 1.0, 1.0);
	float  specularExponent = 10;

	vec3 lightRay   = normalize( lightPos  - Pos );
	vec3 cameraRay  = normalize( cameraPos - Pos );
	vec3 halfVector = normalize( lightRay + cameraRay );

	vec3 ambient  = ambientRC * ambientLight;
	vec3 diffuse  =     max(dot(lightRay,   Normal), 0) * vec3(1, 1, 1);// * objColor;
	vec3 specular = pow(max(dot(halfVector, Normal), 0), specularExponent) * specularLight;

	vec3 col = (ambient + diffuse + specular) * objColor;

//	FragColor = (lightPos.y > 3000) ? vec4(1, 0, 0, 1) : vec4(0, 1, 0, 1);
	FragColor = vec4(col.xyz, 1.0);
} 
