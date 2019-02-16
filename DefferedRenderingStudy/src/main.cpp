#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
    ofGLWindowSettings s;
    s.setGLVersion(3, 3);
    s.setSize(800, 600);
//    s.resizable = false;
    ofCreateWindow(s);

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());

}
