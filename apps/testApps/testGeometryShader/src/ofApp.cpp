#include "ofApp.h"


bool ofApp::reloadShader(){

	glGetError();
	bool success(true);

	success &= mHairShader.setupShaderFromFile( GL_VERTEX_SHADER, "basic_VS.glsl");
	success &= mHairShader.setupShaderFromFile( GL_FRAGMENT_SHADER, "basic_FS.glsl");
	success &= mHairShader.setupShaderFromFile( GL_GEOMETRY_SHADER, "basic_GS.glsl");
	mHairShader.bindDefaults();
	mHairShader.linkProgram();

	return success;

}


//--------------------------------------------------------------
void ofApp::setup(){

	ofSetLogLevel(OF_LOG_VERBOSE);
	ofSetVerticalSync(false);

	mReloadShader = true; 
	mSphereMesh = ofSpherePrimitive().getMesh();

	mCam.setAutoDistance(false);
	mCam.setPosition(10,10,-60);
	mCam.lookAt(ofVec3f(0,0,0));

	// init opengl stuff 
	glGenVertexArrays(1, &mVao);
	glBindVertexArray(mVao);
	
	glGenBuffers(NUM_BUFFERS, mBuffers); 


	auto vertices = mSphereMesh.getVertices();
	auto normals = mSphereMesh.getNormals();

	const unsigned int NUM_HAIR_POINTS = 3; 
	const float HAIR_LENGHT = 3.0f;
	std::vector<ofVec3f> hairVertices; 
	std::vector<unsigned int> hairIndices; 
	hairVertices.reserve( NUM_HAIR_POINTS * vertices.size() );
	hairIndices.reserve( NUM_HAIR_POINTS * vertices.size() +  vertices.size());  // we need a bigger one because we add a restart index after each hair
	
	int unsigned index = 0;
	for (int i = 0; i <  vertices.size(); i++)
	{

		ofVec3f currentVertex = vertices.at(i);
		ofVec3f currentNormal = normals.at(i);
		currentNormal.normalize();
		hairIndices.push_back(index ); // we use adjacency line strips, first hair vertex is twice
		for( int j = 0; j < NUM_HAIR_POINTS; j++){
	
			ofVec3f v = currentVertex + currentNormal * HAIR_LENGHT * j/NUM_HAIR_POINTS;
			hairVertices.push_back( v);
			hairIndices.push_back(index ); 
			if( j == NUM_HAIR_POINTS-1)
				hairIndices.push_back(index ); // we use adjacency line strips, last hair vertex is twice

			index ++; 
		}
		hairIndices.push_back(RESTART_INDEX ); 

	}

	mNumVertices = hairVertices.size();
	mNumIndices = hairIndices.size();
	
	glBindBuffer( GL_ARRAY_BUFFER, mBuffers[POS_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ofVec3f) * hairVertices.size(), &hairVertices[0], GL_STATIC_DRAW );
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,3,GL_FLOAT, GL_FALSE,0,0);
//	glBindBuffer( GL_ARRAY_BUFFER,0);

	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mBuffers[INDEX_BUFFER]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * hairIndices.size(), &hairIndices[0], 
                    GL_STATIC_DRAW);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

//	GLint UniformBufferOffset(0);
//	glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &UniformBufferOffset);
//	GLint UniformBlockSize = std::max(GLint(sizeof(ofMatrix4x4)), UniformBufferOffset);
//
//	glBindBuffer(GL_UNIFORM_BUFFER,  mBuffers[TRANSFORM]);
//	glBufferStorage(GL_UNIFORM_BUFFER, UniformBlockSize, NULL, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
//
////	glBindBuffer(GL_UNIFORM_BUFFER, 0);


	glBindVertexArray(0);









}

//--------------------------------------------------------------
void ofApp::update(){




}

//--------------------------------------------------------------
void ofApp::draw(){
	
	if(mReloadShader){
		mReloadShader = false;
		if( ! reloadShader() )
			return; 

	}

	ofEnableDepthTest();
	ofClear(0,0,0,0);

	mCam.begin();

	ofSetColor(255,0,0,255);
	ofRotateX( ofGetFrameNum() * 0.003f);
	ofRotateZ( ofGetFrameNum() * 0.001f);

	mSphereMesh.drawWireframe();
	
	
	mHairShader.begin();

	//mHairShader.setUniformMatrix4f("inverseModelViewMatrix", mCam.getModelViewMatrix().getInverse());
	mHairShader.setUniform3f("cameraPosition", mCam.getGlobalPosition() );
	ofSetColor(255,255,0,255);

	glEnable( GL_PRIMITIVE_RESTART);
	glPrimitiveRestartIndex(RESTART_INDEX);
	glBindVertexArray( mVao);

	//glDrawArrays(GL_LINE_STRIP, 0, mNumVertices);
	//glDrawElements(GL_LINE_STRIP, mNumIndices, GL_UNSIGNED_INT, 0);
	glDrawElements(GL_LINE_STRIP_ADJACENCY, mNumIndices, GL_UNSIGNED_INT, 0);
	glDisable( GL_PRIMITIVE_RESTART);
	
	glBindVertexArray( 0);

	mHairShader.end();

	mCam.end();

	ofDisableDepthTest();
	ofSetColor(0, 255, 0);
    string framerate = ofToString( ofGetFrameRate() );
    ofDrawBitmapString( framerate, 30, 30);
    ofSetColor(255, 255, 255);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){



	switch (key)
	{

		case 'r':
			mReloadShader = true;
			break;
		default:
			break;
	}

}




//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
