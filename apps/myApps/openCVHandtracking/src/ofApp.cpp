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
    logoImg.setFromPixels(logo.getPixels(), 100, 100);
    
	bLearnBakground = true;
	threshold = 80;
    
    //-- Step 1: Detect the keypoints using SURF Detector
    int minHessian = 1000;
    detector = SurfFeatureDetector( minHessian );
    detector.detect( logoImg.getCvImage(), keypoints_logo );
    extractor.compute( logoImg.getCvImage(), keypoints_logo, des_logo );

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
        // grayImg.setFromColorImage(colorImg);
        detector.detect( colorImg.getCvImage() , keypoints_video );
        extractor.compute( colorImg.getCvImage(), keypoints_video, des_image );
        matcher.knnMatch(des_logo, des_image, matches, 2);
        
        
        for(int i = 0; i < min(des_image.rows-1,(int) matches.size()); i++) //THIS LOOP IS SENSITIVE TO SEGFAULTS
            {
            if((matches[i][0].distance < 0.6*(matches[i][1].distance)) && ((int) matches[i].size()<=2 && (int) matches[i].size()>0))
                {
                good_matches.push_back(matches[i][0]);
                }
            }
        
        

        for( int i = 0; i < good_matches.size(); i++ )
            {
            //Get the keypoints from the good matches
            logo.push_back( keypoints_logo[ good_matches[i].queryIdx ].pt );
            video.push_back( keypoints_video[ good_matches[i].trainIdx ].pt );
            }
        //Mat H;
        //H = findHomography( obj, scene, CV_RANSAC );
        
        //perspectiveTransform( obj_corners, scene_corners, H);
        

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
    logoImg.draw(400, 400);

    int i = 0;
    for( auto p : video){
        cv::Point2f p2 = logo.at(i);
        ofLine(20+p.x,20+ p.y,400 +p2.y,400+ p2.y);
        
    }


    logo.clear();
    video.clear();

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
