#include "ofApp.h"

void ofApp::createGBuffer() {
    glGenFramebuffers(1, &gBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
    
    // position color buffer
    glGenTextures(1, &gPosition);
    glBindTexture(GL_TEXTURE_2D, gPosition);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, ofGetWidth(), ofGetHeight(), 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);
    
    // normal color buffer
    glGenTextures(1, &gNormal);
    glBindTexture(GL_TEXTURE_2D, gNormal);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, ofGetWidth(), ofGetHeight(), 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);
    
    // position color buffer
    glGenTextures(1, &gColorSpec);
    glBindTexture(GL_TEXTURE_2D, gColorSpec);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, ofGetWidth(), ofGetHeight(), 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gColorSpec, 0);
    
    unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
    glDrawBuffers(3, attachments);
    
//    GLuint rboDepth;
//    glGenRenderbuffers(1, &rboDepth);
//    glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
//    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, ofGetWidth(), ofGetHeight());
//    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
    
//    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
//        std::cout << "Framebuffer not complete!" << std::endl;
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

//--------------------------------------------------------------
void ofApp::setup(){
    ofDisableArbTex();
    // gBuffer settings
    createGBuffer();
    
    gBufferShader.load("shader/gBuffer");
    renderShader.load("shader/render");
    
    for(int i=0; i<NUM; i++) {
        float x = ofRandom(ofGetWidth()) - (ofGetWidth() / 2);
        float y = ofRandom(ofGetHeight()) - (ofGetHeight() / 2);
        float z = ofRandom(200.0) - 100.0;
        pos[i] = ofVec3f(x, y, z);
    }
    
    sphereMesh = ofSpherePrimitive(50, 4).getMesh();
    
    quad.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
    quad.addVertex(ofVec3f(ofGetWidth()/2, ofGetHeight()/2, 0.0));
    quad.addTexCoord(ofVec2f(1.0f, 1.0f));
    quad.addVertex(ofVec3f(ofGetWidth()/2, 0.0, 0.0));
    quad.addTexCoord(ofVec2f(1.0f, 0.0f));
    quad.addVertex(ofVec3f(0.0, 0.0, 0.0));
    quad.addTexCoord(ofVec2f(0.0f, 0.0f));
    quad.addVertex(ofVec3f(0.0, ofGetHeight()/2, 0.0));
    quad.addTexCoord(ofVec2f(0.0f, 1.0f));
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    cam.begin();
    ofMatrix4x4 viewMatrix = ofGetCurrentViewMatrix();
    cam.end();
    
    gBufferShader.begin();
    gBufferShader.setUniformMatrix4f("view", viewMatrix);
    gBufferShader.setUniformMatrix4f("projection", cam.getProjectionMatrix());
    for(int i=0; i<NUM; i++) {
        ofMatrix4x4 modelMatrix;
        modelMatrix.translate(pos[i]);
        gBufferShader.setUniformMatrix4f("model", modelMatrix);
        
        sphere.draw();
    }
    gBufferShader.end();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    

    // rendering
    renderShader.begin();
    renderShader.setUniformTexture("gPosition", GL_TEXTURE_2D, gPosition, 0);
    renderShader.setUniformTexture("gNormal", GL_TEXTURE_2D, gNormal, 1);
    renderShader.setUniformTexture("gColorSpec", GL_TEXTURE_2D, gColorSpec, 2);
    renderShader.setUniform1i("debugMode", debugMode);
    quad.draw(OF_MESH_FILL);
    renderShader.end();

    ofDrawBitmapString("debugMode: " + ofToString(debugMode), 10, 10);
//    glBindTexture(GL_TEXTURE_2D, gPosition);
//    quad.draw(OF_MESH_FILL);
    
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch(key) {
        case '0':
            debugMode = 0;
            break;
        case '1':
            debugMode = 1;
            break;
        case '2':
            debugMode = 2;
            break;
        case '3':
            debugMode = 4;
        default:
            break;
    }
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
