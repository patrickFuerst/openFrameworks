#pragma once

#include "ofMain.h"

#define INDEX_BUFFER 0 
#define POS_VB 1
#define TRANSFORM 2
#define NUM_BUFFERS 3

#define RESTART_INDEX 65535

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();


		bool reloadShader();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);



		ofShader mHairShader; 
		bool mReloadShader;


		GLuint mBuffers[NUM_BUFFERS];
		int mNumVertices; 
		int mNumIndices; 
		GLuint mVao; 

		ofMesh mSphereMesh; 

		ofEasyCam mCam; 
		
};
