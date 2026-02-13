#version 330 core

out vec4 FragColor;

in vec4 vertexColor;
in vec2 texCoords;
in vec3 normal;
in vec3 fragPos;

uniform sampler2D texture1;
uniform sampler2D texture2;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
	// ambient
	float ambientIntensity = 0.1f;
	vec3 ambient = ambientIntensity * lightColor;
	// diffuse
	vec3 normalizedNormal = normalize(normal);
	vec3 lightDirection = normalize(lightPos - fragPos);
	float diff = max(dot(normalizedNormal, lightDirection), 0.0f);
	vec3 diffuse = lightColor * diff;
	// specular
	float specularStrength = 0.5f;
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDirection, normalizedNormal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 32);
	vec3 specular = spec * lightColor * specularStrength;
	FragColor = vec4(objectColor * (ambient + diffuse + specular), 1.0f);
}