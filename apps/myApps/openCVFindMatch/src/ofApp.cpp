#include "ofApp.h"



using namespace cv;
//--------------------------------------------------------------
void ofApp::setup(){

        vidGrabber.setVerbose(true);
        vidGrabber.initGrabber(320*2,240*2);
	

    colorImg.allocate(320*2,240*2);
    logoImg.allocate(100,100);


    ofPixels logo;
    ofLoadImage(logo, "tracklogo.png");
    logoImg.setFromPixels(logo.getPixels(), 20, 20);
    grayImgLogo.setFromColorImage(logoImg);
	bLearnBakground = true;
	threshold = 80;
    imgResult = cvCreateImage(cvSize(320*2 - 20 + 1,240*2 - 20 +1),IPL_DEPTH_32F,1);

    
}

//--------------------------------------------------------------
void ofApp::update(){
	ofBackground(100,100,100);

    bool bNewFrame = false;

       vidGrabber.update();
	   bNewFrame = vidGrabber.isFrameNew();

    Mat des_image, img_matches;
    std::vector<vector<DMatch > > matches;
    std::vector<DMatch > good_matches;
	if (bNewFrame){

        colorImg.setFromPixels(vidGrabber.getPixels(), 320*2, 240*2);
        grayImg.setFromColorImage(colorImg);
        
        cvMatchTemplate(grayImg.getCvImage(), grayImgLogo.getCvImage(), imgResult,CV_TM_CCORR_NORMED);
        
        double min_val=0.2, max_val=0.8;
        CvPoint min_loc, max_loc;
        cvMinMaxLoc(imgResult, &min_val, &max_val, &min_loc, &max_loc);

        cvRectangle(colorImg.getCvImage(), max_loc, cvPoint(max_loc.x+grayImgLogo.getCvImage()->width,  max_loc.y+grayImgLogo.getCvImage()->height), cvScalar(0), 1);

	}
    
    

    
    

    // drawKeypoints( colorImg.getCvImage(), keypoints_1, &colorImg.getCvImage(), Scalar::all(-1), DrawMatchesFlags::DEFAULT );
    //drawKeypoints( logoImg.getCvImage(), keypoints_2, img2, Scalar::all(-1), DrawMatchesFlags::DEFAULT );
    
    //-- Show detected (drawn) keypoints
//    imshow("Keypoints 1", img_keypoints_1 );
//    imshow("Keypoints 2", img_keypoints_2 );

}

//--------------------------------------------------------------
void ofApp::draw(){

    
	// draw the incoming, the grayscale, the bg and the thresholded difference
	ofSetHexColor(0xffffff);
	colorImg.draw(20,20);
    logoImg.draw(400, 400,100,100);
   
}



//--------------------------------------------------------------
void ofApp::keyPressed(int key){

	switch (key){
		case ' ':
			bLearnBakground = true;
			break;
		case '+':
			threshold ++;
			if (threshold > 255) threshold = 255;
			break;
		case '-':
			threshold --;
			if (threshold < 0) threshold = 0;
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
