#version 330 core

uniform float someUniform;

out vec4 FragColor;

in vec4 vertexColor;

void main()
{
	FragColor = vertexColor;
}