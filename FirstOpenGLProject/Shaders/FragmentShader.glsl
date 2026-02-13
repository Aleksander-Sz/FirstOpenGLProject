#version 330 core

out vec4 FragColor;

in vec4 vertexColor;
in vec2 texCoords;

uniform sampler2D texture1;
uniform sampler2D texture2;

uniform vec3 objectColor;
uniform vec3 lightColor;

void main()
{
	FragColor = vec4(objectColor * lightColor, 1.0f);
}