#pragma once

#include "ofMain.h"

#include "ofxOpenCv.h"



using namespace cv;


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);		

		  ofVideoGrabber 		vidGrabber;
		
    ofxCvColorImage			colorImg;
    ofxCvGrayscaleImage     grayImg;
    ofxCvColorImage			logoImg;
    std::vector<KeyPoint> keypoints_video, keypoints_logo;
    SurfFeatureDetector detector;
    SurfDescriptorExtractor extractor;
    FlannBasedMatcher matcher;
    Mat des_logo;
    std::vector<Point2f> logo;
    std::vector<Point2f> video;

		int 				threshold;
		bool				bLearnBakground;


};

