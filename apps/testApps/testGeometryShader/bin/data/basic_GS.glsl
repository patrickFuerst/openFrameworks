#version 440 core


#define POSITION	0
#define COLOR		1
#define NORMAL		2
#define TEXCOORD    3
#define FRAG_COLOR	0

precision highp float;
precision highp int;
//layout(std140, column_major) uniform;
//layout(std430, column_major) buffer; AMD bug

uniform mat4 projectionMatrix;
uniform mat4 inverseModelViewMatrix;
uniform vec3 cameraPosition;

layout(lines_adjacency) in; 
layout(triangle_strip,  max_vertices = 4) out; // two triangles, to create a plane facing to camera

in block
{
	vec4 color;
} In[];

out block
{
	vec4 color;
} Out;

in gl_PerVertex
{
    vec4 gl_Position;
    //float gl_PointSize;
   // float gl_ClipDistance[];
} gl_in[];



void main()
{

	// position is in view space coords
	vec4 pos1 = gl_in[1].gl_Position;
	vec4 pos2 = gl_in[2].gl_Position;
	vec3 tangent = pos2.xyz - pos1.xyz;
	tangent = normalize(tangent);
	vec3 eyeVec =   normalize(-pos1.xyz);
	vec3 sideVec = normalize(cross(eyeVec, tangent));
	//vec3 width1 = sideVec * strandWidth[0];
	//vec3 width2 = sideVec * strandWidth[1];
    vec3 width1 = sideVec * 0.25;
	vec3 width2 = sideVec * 0.25;
	// Offset positions to for drawing triangles in world space
	gl_Position =  projectionMatrix *   vec4( pos1.xyz + width1, 1 ) ;
   	Out.color = In[1].color;
    EmitVertex();

	gl_Position =  projectionMatrix *  vec4( pos1.xyz - width1, 1 );
	Out.color = In[1].color;
    EmitVertex();

	gl_Position =  projectionMatrix *  vec4( pos2.xyz + width2, 1 );
   	Out.color = In[2].color;
    EmitVertex();

	gl_Position =  projectionMatrix *  vec4( pos2.xyz - width2, 1 );
   	Out.color = In[2].color;
    EmitVertex();

   // EndPrimitive();
}

