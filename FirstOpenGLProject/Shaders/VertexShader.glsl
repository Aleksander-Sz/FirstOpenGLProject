#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 textureCoordinates;

out vec4 vertexColor;
out vec2 texCoords;

uniform float offset;

void main()
{
	gl_Position = vec4(aPos.x, aPos.y + offset, aPos.z, 1.0);
	vertexColor = vec4((aPos.x+1)/2, (aPos.y+1)/2, 0.2f, 1.0f);
	texCoords = textureCoordinates;
}