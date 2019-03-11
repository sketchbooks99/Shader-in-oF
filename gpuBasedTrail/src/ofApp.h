#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "Trail.hpp"
#include "Blur.hpp"

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
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
        void audioIn(ofSoundBuffer & input);
    
    int trailLength, numTrail;
    
    ofLight light;
    
    ofEasyCam cam;
    ofVboMesh mesh;
    
    Trail trail;
    
    Blur blur;
    
    ofxPanel gui;
    ofParameter<float> trailWidth;
    ofParameter<float> speed;
    ofParameter<float> size;
    ofParameter<float> blurSize;
    ofParameter<float> blurStrength;
    bool isGui = true, isSpeedUp = true;
    
    
    // sound
    vector<float> left;
    vector<float> right;
    
    int bufferCounter;
    
    float smoothedVol;
    float scaledVol;
    
    ofSoundStream soundStream;
};
