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
    
    
    
    mTessellationShader.setupShaderFromFile(GL_VERTEX_SHADER, "tess.vert");
    mTessellationShader.setupShaderFromFile(GL_FRAGMENT_SHADER, "tess.frag");
    mTessellationShader.setupShaderFromFile(GL_GEOMETRY_SHADER, "tess.geom");
    mTessellationShader.setupShaderFromFile(GL_TESS_CONTROL_SHADER, "tess.cont");
    mTessellationShader.setupShaderFromFile(GL_TESS_EVALUATION_SHADER, "tess.eval");
    mTessellationShader.linkProgram();
    
    mTessellationLevel = 4;
    mMousePos = ofVec2f();
    
    
    mVbo.enableVAOs();
    mVbo.enableColors();
    mVbo.enableNormals();
    //    mVbo.setVertexData(vertexData, 4, GL_STATIC_DRAW);
    //    mVbo.setColorData(colorData, 4, GL_STATIC_DRAW);
    
    //mVbo.setMesh(ofMesh::sphere(1.0f, 4, OF_PRIMITIVE_TRIANGLES), GL_STATIC_DRAW, true, false, false);
        mVbo.setMesh( ofMesh::plane(2, 2,4,4, OF_PRIMITIVE_TRIANGLES), GL_STATIC_DRAW, true, false, false);

    
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
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // mCam.begin();
    mTessellationShader.begin();
    //mTessellationShader.setUniformMatrix4f("MVP", mCam.getModelViewProjectionMatrix());
    mTessellationShader.setUniform1i("tessellationLevel", mTessellationLevel);
    mTessellationShader.setUniform2fv("mousePos", &mMousePos[0]);
    mVbo.bind();
    glPatchParameteri(GL_PATCH_VERTICES, 3);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVbo.getIndexId());
    mVbo.drawElements(GL_PATCHES, mVbo.getNumIndices());
    //  mVbo.drawInstanced(GL_PATCHES, 0, mVbo.getNumVertices(), 1);
    mTessellationShader.end();
    

    //  mCam.end();
    

    drawInfo();

}




void ofApp::drawInfo(){
    
    
    string info =
    
    "Press Key:\n"
    " +: increase tessellation level \n"
    " -: decrease tessellation level  \n"
    " Level: " + ofToString(mTessellationLevel) +"\n"
    "\n"
    "///////  ///////     ofTestTessellation \n"
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
        case '+':
            if(mTessellationLevel < 16 ) mTessellationLevel++;
            break;
        case '-':
            if(mTessellationLevel > 1 ) mTessellationLevel--;
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

    mMousePos = ofVec2f((float)x/ofGetWindowWidth(),(float)y/ofGetWindowHeight());
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
