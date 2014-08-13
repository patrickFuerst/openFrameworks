#version 400 core

precision highp float;
precision highp int;
layout(std140, column_major) uniform;
layout(triangles ) in;
layout(triangle_strip, max_vertices =3) out;

in vertex
{
    vec4 Color;
    vec3 Normal;
    vec3 TexCoord;
} In[];

out vertex
{
    vec4 Color;
    vec3 Normal;
    vec3 TexCoord;
} Out;

void main()
{	
	for(int i = 0; i < gl_in.length(); ++i)
	{
		gl_Position = gl_in[i].gl_Position;
		Out.Color = In[i].Color;
		EmitVertex();
	}
	EndPrimitive();
}

