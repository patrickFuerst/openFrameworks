#version 440

#pragma include "computeVariables.glsl"


vec4 updatePosition(  vec4 currentPosition, vec4 velocity ){

	currentPosition.xyz += velocity.xyz * g_timeStep;
	return currentPosition;
}

vec4 updateVelocity(  vec4 currentVelocity,  vec4 force){

	force.xyz  += g_gravityForce;
	currentVelocity.xyz +=  force.xyz * g_timeStep;
	currentVelocity *= 0.99; // damping
	return currentVelocity;
}

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


void  applyLengthConstraint( inout vec4 pos0 , bool fixed0, inout vec4 pos1, bool fixed1,  float targetLength, float stiffness = 1.0){

	vec3 delta = pos1.xyz - pos0.xyz; 
	float distance = max( length( delta ), 1e-7);
	float stretching  = 1.0 - targetLength / distance; 
	delta = stretching * delta; 
	vec2 multiplier = constrainMultiplier(fixed0, fixed1);

	pos0.xyz += multiplier[0] * delta * stiffness;
	pos1.xyz -= multiplier[1] * delta * stiffness;

}


void main(){
	
	sharedPos[gl_LocalInvocationID.x] =  p2[gl_GlobalInvocationID.x].pos;
	memoryBarrierShared();

	vec4 currentVelocity = p2[gl_GlobalInvocationID.x].vel;
	bool fix = p2[gl_GlobalInvocationID.x].fix;
	//vec4 oldPosition = currentPosition;
	
	if( !fix){

		currentVelocity = updateVelocity( currentVelocity ,vec4(0,0,0,0));
		sharedPos[gl_LocalInvocationID.x] = updatePosition( sharedPos[gl_LocalInvocationID.x] ,currentVelocity);
		//currentVelocity.xyz =  (oldPosition.xyz - currentPosition.xyz)/ g_timeStep  * 0.99;

	}else{

		sharedPos[gl_LocalInvocationID.x] = g_modelMatrix * sharedPos[gl_LocalInvocationID.x] ;


	}
	uint index0 = gl_GlobalInvocationID.x;
	uint index1 = gl_GlobalInvocationID.x+1;

	memoryBarrierShared();

	if(  gl_LocalInvocationID.x < 20 ){

		bool fix1 = p2[index1].fix;

		for(int i = 0 ; i < 25 ; i++){
			applyLengthConstraint( sharedPos[gl_LocalInvocationID.x], fix, sharedPos[gl_LocalInvocationID.x+1], fix1, 0.01, 1);

		}

		//p[gl_GlobalInvocationID.x+1].pos.xyz = sharedPos[gl_LocalInvocationID.x+1].xyz;

	}

	p[gl_GlobalInvocationID.x].pos.xyz = sharedPos[gl_LocalInvocationID.x].xyz;

//	p[gl_GlobalInvocationID.x].color = vec4(1,0,1,1) ;
	p[gl_GlobalInvocationID.x].fix = fix;
	//p[gl_GlobalInvocationID.x].vel.xyz = currentVelocity.xyz;
	

}