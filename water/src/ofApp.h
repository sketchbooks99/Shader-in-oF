#pragma once

#include "ofMain.h"

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

		unsigned int loadCubeMap(string name[]);
		void drawCube(int width);
		void createMesh();

		const int width = 256;

		ofEasyCam cam;
		ofTexture tex;
		ofImage image[6];

		unsigned int cubeTexture;

    ofVboMesh boxFaces[6];
    ofVboMesh planeMesh, sphereMesh;

    ofFbo renderFbo;
    ofShader planeShader, sphereShader;

};
