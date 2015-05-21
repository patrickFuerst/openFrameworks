#version 440

#pragma include "computeVariables.glsl"



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
	
	vec4 position[20];
	bool fixed12[20];
	// load data from strain to memory 
	for(uint i =0; i <  g_numVerticesPerStrand; i++){
		position[i] =  p2[ gl_WorkGroupID.x * gl_WorkGroupSize.x + i].pos;
		fixed12[i] =  p2[gl_WorkGroupID.x * gl_WorkGroupSize.x + i].fix;
	}
	//memoryBarrierShared();  


	for(uint solverIterations = 0 ; solverIterations < 25; solverIterations++){
		
		for(int j = 0 ; j < g_numVerticesPerStrand-1 ; j++){
			applyLengthConstraint( position[j], fixed12[j], position[j+1], fixed12[j+1], 0.1, 1);
		}

		//p[gl_GlobalInvocationID.x+1].pos.xyz = sharedPos[gl_LocalInvocationID.x+1].xyz;
	}

	for(uint i =0; i <  g_numVerticesPerStrand; i++){
		 p[gl_WorkGroupID.x * gl_WorkGroupSize.x + i].pos.xyz = position[i].xyz;
	}

//	p[gl_GlobalInvocationID.x].color = vec4(1,0,1,1) ;
	//p[gl_GlobalInvocationID.x].vel.xyz = currentVelocity.xyz;
	

}