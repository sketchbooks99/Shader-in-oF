#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
    
    ofGLFWWindowSettings ss;
    ss.stencilBits = 1;
    ss.setGLVersion(3, 2);
    ss.setSize(1024, 768);
    ofCreateWindow(ss);

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());

}
