#version 440

#pragma include "computeHelper.glsl"

#define LOCAL_GROUP_SIZE_X 64

struct Particle{
	vec4 pos;
	vec4 prevPos;
	vec4 color;
	bool fix;
};

layout(std140, binding=0) buffer particle{
    Particle p[];
};


// model properties
uniform mat4 g_modelMatrixDelta; // need this to transform the fixed vertices if model moves
uniform vec3 g_gravityForce;

// strand properties
uniform float g_stiffness;
uniform float g_strandLength;

// simulation properties
uniform float g_velocityDamping; 
uniform int g_numIterations; 
uniform float g_timeStep;

// additional compute shader properties
uniform int g_numVerticesPerStrand;
uniform int g_numStrandsPerThreadGroup;

shared vec4 sharedPos[LOCAL_GROUP_SIZE_X];
shared bool sharedFixed[LOCAL_GROUP_SIZE_X];

layout(local_size_x = LOCAL_GROUP_SIZE_X, local_size_y = 1, local_size_z = 1) in;


vec4 verletIntegration(  vec4 position , vec4 previousPosition, vec4 force,  bool fix){
	//  TODO implement time correct verlet integration
	if( !fix){

		position.xyz +=   (position.xyz - previousPosition.xyz) * g_velocityDamping	  + force.xyz * g_timeStep * g_timeStep;

	}else{

		position = g_modelMatrixDelta * position ;
		position /= position.w;

	}
	return position; 
}

void main(){
	
	uint localStrandIndex, localVertexIndex, globalStrandIndex, vertexIndexInStrand; 
	calculateIndices( localVertexIndex, localStrandIndex, globalStrandIndex,vertexIndexInStrand, g_numVerticesPerStrand,g_numStrandsPerThreadGroup );
	
	const vec4 oldPosition = sharedPos[localVertexIndex] =  p[gl_GlobalInvocationID.x].pos;
	const vec4 prevPosition =   p[gl_GlobalInvocationID.x].prevPos;
	sharedFixed[localVertexIndex] = p[gl_GlobalInvocationID.x].fix;

	
//	memoryBarrierShared();
	vec4 force = vec4(g_gravityForce,0);

	sharedPos[localVertexIndex]  = verletIntegration( sharedPos[localVertexIndex], prevPosition, force, sharedFixed[localVertexIndex]);


	const uint index0 = localVertexIndex*2;
	const uint index1 = localVertexIndex*2+1;
	const uint index2 = localVertexIndex*2+2;
	const bool fixed0 = sharedFixed[index0];
	const bool fixed1 = sharedFixed[index1];
	const bool fixed2 = sharedFixed[index2];

	memoryBarrierShared();
	
	float stiffness = 1.0 - pow( (1.0 - g_stiffness), 1.0/g_numIterations); // linear depended on the iterations now
	
	for(int i = 0 ; i < g_numIterations ; i++){
		
		if( localVertexIndex <  floor(gl_WorkGroupSize.x/2) && (index0 % g_numVerticesPerStrand) < g_numVerticesPerStrand-1){
			applyLengthConstraint( sharedPos[index0], fixed0, sharedPos[index1], fixed1, g_strandLength/g_numVerticesPerStrand, stiffness);
		}

		groupMemoryBarrier();
		if( (index1 % g_numVerticesPerStrand) < g_numVerticesPerStrand -1){
			applyLengthConstraint( sharedPos[index1], fixed1, sharedPos[index2], fixed2, g_strandLength/g_numVerticesPerStrand, stiffness);			
		}
		groupMemoryBarrier();

	}


	//memoryBarrierShared();

	p[gl_GlobalInvocationID.x].pos.xyz = sharedPos[gl_LocalInvocationID.x].xyz;
	p[gl_GlobalInvocationID.x].prevPos.xyz = oldPosition.xyz;
	

}