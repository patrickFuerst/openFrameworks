#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp{

	public:




		ofShader mShader; 
		vector<GLuint> mSubRoutineUniforms; 

		void setup(){
			
			    const char  vs_source[] =
				{
					"#version 430 core                             \n"
					"                                              \n"
					"layout(location = 0) in vec4 position; \n"
					"void main(void)                               \n"
					"{                                             \n"
					"    gl_Position = position;   \n"
					"}                                             \n"
				};


				const char  fs_source[] =
				{
					"#version 430 core                             \n"
					"                                              \n"
					" subroutine vec4 subColor();                   \n"
					" subroutine uniform subColor u_selectedColor;   \n"
					"                                              \n"
					" subroutine(subColor) vec4 redColor(){        \n"
					"	return vec4(1.0,0.0,0.0,1.0);}             \n"
					" subroutine(subColor) vec4 greenColor(){        \n"
					"	return vec4(0.0,1.0,0.0,1.0);}             \n"
					"                                              \n"
					"                                              \n"
					" out vec4 color;                              \n"
					"void main(void)                               \n"
					"{                                             \n"
					"    color =  u_selectedColor();			   \n"
					"}                                             \n"
				};
		
				mShader.setupShaderFromSource(GL_VERTEX_SHADER, vs_source);
				mShader.setupShaderFromSource(GL_FRAGMENT_SHADER, fs_source);
				mShader.linkProgram();
				

				// let ofShader handle this  for all shadertypes after the program is linked
				GLsizei uniformSize; 
				glGetProgramStageiv( mShader.getProgram(), GL_FRAGMENT_SHADER, GL_ACTIVE_SUBROUTINE_UNIFORM_LOCATIONS, &uniformSize);
				mSubRoutineUniforms.resize(uniformSize);
				
				// already implemented this, so it get cached like normal uniforms 
				GLuint subroutineUniformLocation = glGetSubroutineUniformLocation( mShader.getProgram(), GL_FRAGMENT_SHADER, "u_selectedColor"); // ofShader::getSubroutineUniformLocation
				GLuint subroutineLocation = glGetSubroutineIndex( mShader.getProgram(), GL_FRAGMENT_SHADER, "greenColor" ); // ofShader::getSubroutineLocation
				
				// ofShader should handle an array for all shadetypes 
				mSubRoutineUniforms.at(subroutineUniformLocation) = subroutineLocation; 
		
		};
		void update(){};
		void draw(){
		
			mShader.begin();
			// maybe set this in begin() if mSubRoutineUniforms is not empty
			glUniformSubroutinesuiv( GL_FRAGMENT_SHADER, mSubRoutineUniforms.size(), mSubRoutineUniforms.data() );
			ofDrawSphere(1);

			mShader.end();
		
		
		
		};


		void keyPressed(int key){
		
		
			if( key == 'r'){
				// this could be void setSubroutineUniform( GLenum shadertype, string uniformName, string subroutineName )
				GLuint subroutineUniformLocation = glGetSubroutineUniformLocation( mShader.getProgram(), GL_FRAGMENT_SHADER, "u_selectedColor"); 
				GLuint subroutineLocation = glGetSubroutineIndex( mShader.getProgram(), GL_FRAGMENT_SHADER, "redColor" ); 
				mSubRoutineUniforms.at(subroutineUniformLocation) = subroutineLocation; 

			}else if(key == 'g'){
				GLuint subroutineUniformLocation = glGetSubroutineUniformLocation( mShader.getProgram(), GL_FRAGMENT_SHADER, "u_selectedColor"); 
				GLuint subroutineLocation = glGetSubroutineIndex( mShader.getProgram(), GL_FRAGMENT_SHADER, "greenColor" ); 
				mSubRoutineUniforms.at(subroutineUniformLocation) = subroutineLocation; 
			}
		
		
		};

		
};
