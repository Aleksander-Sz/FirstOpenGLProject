#version 330 core

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};
struct Light {
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

out vec4 FragColor;

in vec4 vertexColor;
in vec2 texCoords;
in vec3 normal;
in vec3 fragPos;

uniform vec3 viewPos;
uniform Light light;

uniform Material material;

void main()
{
	//texture
	vec3 diffuseColor = texture(material.diffuse, texCoords).xyz;
	vec3 specularColor = texture(material.specular, texCoords).xyz;
	// ambient
	vec3 ambient = diffuseColor * light.ambient;
	// diffuse
	vec3 normalizedNormal = normalize(normal);
	vec3 lightDirection = normalize(light.position - fragPos);
	float diff = max(dot(normalizedNormal, lightDirection), 0.0f);
	vec3 diffuse = light.diffuse * diff * diffuseColor;
	// specular
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDirection, normalizedNormal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 32);
	vec3 specular = spec * light.specular * specularColor;
	FragColor = vec4(ambient + diffuse + specular, 1.0f);
}