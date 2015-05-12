#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    
    mVbo.setMesh(ofMesh::plane(1,1,1,1), GL_STATIC_DRAW);
    // mVbo.enableVAOs();
    // main camera
    mCam.lookAt(ofVec3f::zero());
    mCam.setPosition(0, 0, 4);
    mCam.setFarClip(100);
    mCam.setNearClip(0.01);
    mCam.setDistance(4);
    mCam.enableMouseInput();
    // mVbo.disableIndices();

}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

    ofClear(0, 0, 0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    mCam.begin();
    mVbo.drawElements(GL_TRIANGLE_STRIP, mVbo.getNumIndices());
    //mVbo.draw(GL_TRIANGLES, 0, mVbo.getNumVertices());
    mCam.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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
