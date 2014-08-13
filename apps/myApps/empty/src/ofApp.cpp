/*
 * ///////  ///////     ofTestTessellation
 * //   //  //          (c)  2013-2014 , Patrick FŸrst
 * //////   ////        patrickfuerst.at
 * //       //
 * //       //
 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */


#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    
    
    // main camera
    mCam.lookAt(ofVec3f::zero());
    mCam.setPosition(0, 0, 4);
    mCam.setFarClip(100);
    mCam.setNearClip(0.01);
    mCam.setDistance(4);
    mCam.enableMouseInput();


}

//--------------------------------------------------------------
void ofApp::update(){
    
    

}

//--------------------------------------------------------------
void ofApp::draw(){

    ofClear(0, 0, 0);
    ofViewport(ofGetWindowRect());
    
    // mCam.begin();
  
    
    
    
    //  mCam.end();
    
    
    drawInfo();

}


void ofApp::drawInfo(){
    
    
    string info =
    
    "Press Key:\n"
    "\n"
    "///////  ///////      \n"
    "//   //  //          patrickfuerst.at \n"
    "//////   //// \n"
    "//       // \n"
    "//       // ";
    
    
    ofSetColor(0, 255, 0);
    ofDrawBitmapString( info, 30, ofGetWindowHeight() - 150);
    ofSetColor(255, 255, 255);
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    switch (key) {
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
