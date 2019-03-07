#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
    ofGLWindowSettings s;
    s.setSize(1024, 768);
    s.setGLVersion(3,3);
    ofCreateWindow(s);
	ofRunApp(new ofApp());

}
