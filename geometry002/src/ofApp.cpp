#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);
    ofSetFrameRate(60);

    icoShader.setGeometryInputType(GL_TRIANGLES);
    icoShader.setGeometryOutputType(GL_TRIANGLE_STRIP);
    icoShader.setGeometryOutputCount(90);
    icoShader.load("shader/icoSphere.vert", "shader/icoSphere.frag", "shader/icoSphere.geom");
    planeShader.load("shader/icoSphere.vert", "shader/icoSphere.frag", "shader/plane.geom");
    
    icoSphere.set(100, 1);

    icoSphereMesh = icoSphere.getMesh();
    
    plane.set(2000, 2000, 10, 10);
    planeMesh = plane.getMesh();
}

//--------------------------------------------------------------
void ofApp::update(){
    float time = ofGetElapsedTimef();
    float radiusX = 1500;
    float radiusZ = 1500;
    ofVec3f camPos = ofVec3f(radiusZ * sin(time), 500, radiusX * sin(time*1.33));
    cam.setPosition(camPos);
    cam.lookAt(glm::vec3(0, 400, 0));
    
    
    
}

//--------------------------------------------------------------
void ofApp::draw(){
//    ofBackground(200, 50, 200, 255);
    ofBackground(0);
    ofEnableDepthTest();

    cam.begin();
    
    ofPushMatrix();
    ofTranslate(0, 400, 0);
    ofRotateDeg(ofGetElapsedTimef()*30.0, 1.0, 1.0, 0.0);

    icoShader.begin();
    icoShader.setUniform1f("time", ofGetElapsedTimef());
    icoSphereMesh.draw();
    icoShader.end();
    
    ofPopMatrix();
    
    ofPushMatrix();
    ofTranslate(0, -700, 0);
    ofScale(5.0, -1.0, 5.0);
    ofRotateXDeg(90.0);
    
    if(isPlaneShader) {
        planeShader.begin();
        planeShader.setUniform1f("time", ofGetElapsedTimef());
    } else {
        icoShader.begin();
        icoShader.setUniform1f("time", ofGetElapsedTimef());
    }
    planeMesh.draw();
    
    if(isPlaneShader) planeShader.end();
    else icoShader.end();
    
    ofPopMatrix();

    cam.end();

    ofDisableDepthTest();
    
    ofDrawBitmapString("camPosition x : "+ofToString(cam.getPosition().x) + " y : " + ofToString(cam.getPosition().y) + " z : "+ofToString(cam.getPosition().z), 10, 10);
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
    isPlaneShader = !isPlaneShader;
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
