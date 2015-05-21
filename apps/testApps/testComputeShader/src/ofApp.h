#pragma once

#include "ofMain.h"
#include "ofBufferObject.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void dirAsColorChanged(bool & dirAsColor);

		void reloadShaders();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
		struct Particle{
			ofVec4f pos;
			ofVec4f prevPos;
			ofFloatColor color;
			int fixed;  // actually bool in glsl
			int pad[3];  // struct in glsl is aligned to multiple of the biggest base alingment, here 16 , so offset of next is 64 not 52
		};

		ofShader mComputeShader;
		//ofShader mConstrainPerStrainComputeShader;

		ofShader mHairshader; 
		vector<Particle> particles;
		ofBufferObject particlesBuffer;//, particlesBuffer2;
		
		ofMatrix4x4 mStrandModelMatrix, mStrandModelMatrixPrevInversed;
		
		GLuint vaoID;
		//ofEasyCam camera;
		ofCamera camera;
		ofVbo vbo;
		ofxPanel gui;
		ofParameter<float> mVelocityDamping,  mStiffness ; 
		ofParameter<int> mNumConstraintIterations ; 
		ofParameterGroup mShaderUniforms;
		ofParameter<float> fps;


		bool mReloadShaders; 
};
