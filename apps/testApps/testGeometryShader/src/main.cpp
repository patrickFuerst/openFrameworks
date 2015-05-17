#include "ofMain.h"
#include "ofGLProgrammableRenderer.h"
#include "ofApp.h"

//========================================================================
int main( ){



	ofGLWindowSettings settings;
	settings.width = 1280;
	settings.height = 720;
	settings.setGLVersion(4, 4);
	ofCreateWindow(settings);

	
	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());

}
