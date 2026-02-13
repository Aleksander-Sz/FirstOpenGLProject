#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 textureCoordinates;

out vec4 vertexColor;
out vec2 texCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	vertexColor = vec4((aPos.x+1)/2, (aPos.y+1)/2, 0.2f, 1.0f);
	texCoords = textureCoordinates;
}