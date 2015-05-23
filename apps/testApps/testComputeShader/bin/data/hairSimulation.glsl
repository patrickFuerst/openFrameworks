#version 440

//#pragma include "computeHelper.glsl"

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
uniform float g_ftlDamping;

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


vec4  applyLengthConstraintDFTL(  vec4 pos0 ,  bool fixed0,  vec4 pos1,  bool fixed1,  float targetLength, float stiffness = 1.0){

	vec3 delta = pos1.xyz - pos0.xyz; 
	float distance = max( length( delta ), 1e-7);
	float stretching  = 1.0 - targetLength / distance; 
	delta = stretching * delta; 
	vec2 multiplier = constrainMultiplier(fixed0, fixed1);

	return vec4(pos1.xyz - 1.0 * delta * stiffness,1.0);

}

void calculateIndices( inout uint localVertexIndex , inout uint localStrandIndex ,
					 inout uint globalStrandIndex , inout uint vertexIndexInStrand,  uint numVerticesPerStrand, 
					 uint numStrandsPerThreadGroup    ){



	localVertexIndex = gl_LocalInvocationID.x; 
	localStrandIndex = uint(floor(gl_LocalInvocationID.x /  numVerticesPerStrand));
	globalStrandIndex = gl_WorkGroupID.x * numStrandsPerThreadGroup + localStrandIndex;
	vertexIndexInStrand = gl_LocalInvocationID.x %  numVerticesPerStrand; 



}

vec4 positionIntegration( vec4 position, vec4 velocity, vec4 force, bool fix){

	if( !fix){

		position.xyz +=   velocity.xyz * g_velocityDamping * g_timeStep  + force.xyz * g_timeStep * g_timeStep;

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
	const vec4 velocity =   p[gl_GlobalInvocationID.x].vel;
	sharedFixed[localVertexIndex] = p[gl_GlobalInvocationID.x].fix;

	
	vec4 force = vec4(g_gravityForce,0);
	sharedPos[localVertexIndex]  = positionIntegration( sharedPos[localVertexIndex], velocity, force, sharedFixed[localVertexIndex]);

	memoryBarrierShared();

	if(vertexIndexInStrand  == 0){

		for(int i= 0; i < g_numVerticesPerStrand-1; i++){
			bool fix = sharedFixed[localVertexIndex+i+1];
			sharedPos[localVertexIndex+i+1] = applyLengthConstraintDFTL( sharedPos[localVertexIndex+i], true, sharedPos[localVertexIndex+i+1], fix, g_strandLength/g_numVerticesPerStrand, g_stiffness);			

		}

	}

 	groupMemoryBarrier();
 	vec4 distanceToNext = vec4(0,0,0,0);
	if(vertexIndexInStrand < g_numVerticesPerStrand-1){
		distanceToNext.xyz = sharedPos[localVertexIndex].xyz - sharedPos[localVertexIndex+1].xyz ;
	}
	vec3 derivedVelocity = (sharedPos[localVertexIndex].xyz - oldPosition.xyz) / g_timeStep - g_ftlDamping *distanceToNext.xyz / g_timeStep; 

	//memoryBarrierShared();

	p[gl_GlobalInvocationID.x].vel.xyz = derivedVelocity.xyz;
	p[gl_GlobalInvocationID.x].pos.xyz = sharedPos[gl_LocalInvocationID.x].xyz;
	p[gl_GlobalInvocationID.x].prevPos.xyz = oldPosition.xyz;
	

}