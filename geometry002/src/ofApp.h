#pragma once

#include "ofMain.h"
#include "ofxBvh.h"
#include "ofxGui.h"

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
		
    ofVboMesh icoSphereMesh, planeMesh;
    ofIcoSpherePrimitive icoSphere;
    ofPlanePrimitive plane;
    
    ofFbo renderFbo, onepassFbo, twopassFbo;
    
    ofShader icoShader;
    ofShader planeShader;
    ofEasyCam cam;
    
    bool isPlaneShader;
    
    ofxBvh bvh;
    vector<glm::vec3> bvhPos;
};
