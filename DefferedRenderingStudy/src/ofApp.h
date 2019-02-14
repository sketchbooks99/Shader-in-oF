#pragma once

#include "ofMain.h"

#define NUM 30

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
    void createGBuffer();
		
    ofShader gBufferShader, renderShader;
    
    unsigned int gBuffer;
    unsigned int gPosition, gNormal, gColorSpec;
    
    ofVboMesh quad, sphereMesh;
    
    int debugMode = 0;
    
    ofSpherePrimitive sphere;
    ofVec3f pos[NUM];
    
    ofEasyCam cam;
};
