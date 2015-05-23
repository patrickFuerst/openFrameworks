#version 440

#pragma include "computeHelper.glsl"

#define LOCAL_GROUP_SIZE_X 64

struct Particle{
	vec4 pos;
	vec4 prevPos;
	vec4 vel;
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


uniform bool g_useFTL; 

shared vec4 sharedPos[LOCAL_GROUP_SIZE_X];
shared bool sharedFixed[LOCAL_GROUP_SIZE_X];

layout(local_size_x = LOCAL_GROUP_SIZE_X, local_size_y = 1, local_size_z = 1) in;

vec2 constrainMultiplier( bool fixed0 , bool fixed1){

	if( fixed0 ){

		if(fixed1)
			return vec2(0,0);
		else 
			return vec2(0,1);
	}else{

		if(fixed1)
			return vec2(1,0);
		else
			return vec2(0.5,0.5);
	}

}


void  applyLengthConstraint( inout vec4 pos0 , in bool fixed0, inout vec4 pos1, in bool fixed1,  float targetLength, float stiffness = 1.0){

	vec3 delta = pos1.xyz - pos0.xyz; 
	float distance = max( length( delta ), 1e-7);
	float stretching  = 1.0 - targetLength / distance; 
	delta = stretching * delta; 
	vec2 multiplier = constrainMultiplier(fixed0, fixed1);

	pos0.xyz += multiplier[0] * delta * stiffness;
	pos1.xyz -= multiplier[1] * delta * stiffness;

}

void calculateIndices( inout uint localVertexIndex , inout uint localStrandIndex ,
					 inout uint globalStrandIndex , inout uint vertexIndexInStrand,  uint numVerticesPerStrand, 
					 uint numStrandsPerThreadGroup    ){



	localVertexIndex = gl_LocalInvocationID.x; 
	localStrandIndex = uint(floor(gl_LocalInvocationID.x /  numVerticesPerStrand));
	globalStrandIndex = gl_WorkGroupID.x * numStrandsPerThreadGroup + localStrandIndex;
	vertexIndexInStrand = gl_LocalInvocationID.x %  numVerticesPerStrand; 



}

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