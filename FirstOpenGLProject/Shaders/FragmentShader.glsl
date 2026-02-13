#version 330 core

out vec4 FragColor;

in vec4 vertexColor;
in vec2 texCoords;

uniform sampler2D texture1;
uniform sampler2D texture2;

uniform float mixingFactor;

void main()
{
	FragColor = mix(texture(texture1, texCoords), texture(texture2, texCoords), mixingFactor);
}