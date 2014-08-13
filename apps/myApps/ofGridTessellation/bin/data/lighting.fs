#version 400 core

#define FRAG_COLOR		0

precision highp float;
precision highp int;
layout(std140, column_major) uniform;

in block
{
	vec4 Color;
	vec3 Normal;
    vec3 WorldPos;
} In;

layout(location = FRAG_COLOR, index = 0) out vec4 FragColor;

void main()
{
	FragColor = vec4(1);
}
