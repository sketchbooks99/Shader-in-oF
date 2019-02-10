#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(0);
    shader.setGeometryInputType(GL_TRIANGLES);
    shader.setGeometryOutputType(GL_TRIANGLE_STRIP);
    shader.setGeometryOutputCount(6);
    shader.load("shader/shader.vert", "shader/shader.frag", "shader/shader.geom");
//    blur.load("shader/passthru.vert", "shader/blur.frag");
//    result.load("shader/passthru.vert", "shader/result.frag");
    reverse.load("shader/passthru.vert", "shader/reverse.frag");
    
    renderFBO.allocate(ofGetWidth(), ofGetHeight());
    
    ofIcoSpherePrimitive icoSphere;
    icoSphere.set(300, 2);
    
    vboMesh = icoSphere.getMesh();
}

//--------------------------------------------------------------
void ofApp::update(){
    
    renderFBO.begin();
    ofClear(0, 0, 0);
    cam.begin();
    ofPushMatrix();
    ofEnableDepthTest();
    
    ofRotateDeg(ofGetElapsedTimef()*30.0, 1.0, 1.0, 0.8);
    
    shader.begin();
    shader.setUniform1f("size", 0.9);
    vboMesh.draw(OF_MESH_FILL);
    shader.end();
    
    ofDisableDepthTest();
    ofPopMatrix();
    cam.end();
    
    renderFBO.end();
}

//--------------------------------------------------------------
void ofApp::draw(){
//    reverse.begin();
//    reverse.setUniformTexture("tex", renderFBO.getTexture(), 0);
//    reverse.setUniform1f("time", ofGetElapsedTimef());
//    reverse.setUniform1i("repTime", repTime);
    renderFBO.draw(0, 0);
//    reverse.end();
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
    if(count == 2) isDown = true;
    if(count == 0) isDown = false;
    
    if(isDown) count--;
    else count++;
    
    switch(count) {
        case 0:
            repTime = 1;
            break;
        case 1:
            repTime = 4;
            break;
        case 2:
            repTime = 9;
            break;
        default:
            break;
    }
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
