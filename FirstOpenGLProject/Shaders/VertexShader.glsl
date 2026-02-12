#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec4 vertexColor;

uniform float offset;

void main()
{
	gl_Position = vec4(aPos.x, aPos.y + offset, aPos.z, 1.0);
	vertexColor = vec4((aPos.x+1)/2, (aPos.y+1)/2, 0.2f, 1.0f);
}