#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(50);
    ofSetVerticalSync(true);
    ofSetFrameRate(60);
    ofEnableAlphaBlending();
    
    numTrail = 4096;
    trailLength = 30;
    
    trail.init(numTrail, trailLength);
    trail.createBuffers(trail.getNum(), trail.getLength());
    trail.loadVelocityShader("shader/passthru.vert", "shader/velUpdate.frag");
    trail.loadPositionShader("shader/passthru.vert", "shader/posUpdate.frag");
    trail.loadRenderShader("shader/createTrail.vert", "shader/createTrail.frag", "shader/createTrail.geom");
    
    gui.setup();
    gui.setPosition(10, 10);
    gui.add(trailWidth.set("Width", 1.0, 0.1, 10.0));
}

//--------------------------------------------------------------
void ofApp::update(){
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
    
    trail.setWidth(trailWidth);
    trail.update();
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofEnableDepthTest();
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    
    cam.begin();
    
    trail.draw(OF_MESH_FILL);
    
    cam.end();
    
    ofDisableDepthTest();
    ofDisableBlendMode();
    
    gui.draw();
//    velPingPong.src->draw(0, 0, ofGetWidth(), ofGetHeight());
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

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
