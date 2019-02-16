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
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, ofGetWidth(), ofGetHeight(), 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gColorSpec, 0);
    
    GLuint depthrenderbuffer;
    glGenRenderbuffers(1, &depthrenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, ofGetWidth(), ofGetHeight());
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);
    
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
    ofBackground(0);
    ofSetVerticalSync(true);
    // gBuffer settings
    createGBuffer();
    
    gBufferShader.load("shader/gBuffer");
    lightingShader.load("shader/lighting");
    
    ofLoadImage(normalMap, "image/normal.jpg");
    
    for(int i=0; i<NUM; i++) {
        pos[i] = ofVec3f(ofRandom(-R, R), ofRandom(-R, R), ofRandom(-R, R));
    }
    
    for(int i=0; i<LIGHT_NUM; i++) {
        ofVec3f lightPos = ofVec3f(ofRandom(-R, R), ofRandom(-R, R), ofRandom(-R, R));
        light[i].setPosition(lightPos);
        light[i].setColor(ofRandom(1.0), ofRandom(1.0), ofRandom(1.0));
        light[i].setRadius(lightPos.length());
    }
    
    vboMesh = ofBoxPrimitive(10, 10, 10).getMesh();
    floor = ofPlanePrimitive(1000, 1000, 10, 10).getMesh();
    
    quad.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
    quad.addVertex(ofVec3f(ofGetWidth(), ofGetHeight(), 0.0));
    quad.addTexCoord(ofVec2f(1.0f, 1.0f));
    quad.addVertex(ofVec3f(ofGetWidth(), 0.0, 0.0));
    quad.addTexCoord(ofVec2f(1.0f, 0.0f));
    quad.addVertex(ofVec3f(0.0, 0.0, 0.0));
    quad.addTexCoord(ofVec2f(0.0f, 0.0f));
    quad.addVertex(ofVec3f(0.0, ofGetHeight(), 0.0));
    quad.addTexCoord(ofVec2f(0.0f, 1.0f));
    
    gui.setup();
    gui.setPosition(10, 10);
    gui.add(debugMode.set("DebugMode", 0, 0, 3));
    gui.add(lightDebug.setup("Light Debug", false));
    gui.add(arrayDebug.setup("Array Debug", false));
    gui.add(lightDistance.set("Light Distance", 100.0, 1.0, 1000.0));
    gui.add(lightIndex.set("Light Index", 0, 0, LIGHT_NUM - 1));
    gui.add(lightAttenuation.set("Attenuation", 0.01, 0.0, 0.3));
    gui.add(disCoef.set("Distance Coefficient", 1.0, 0.0, 1.0));
    
//    cam.setupPerspective(false, 80, 0.1f, 100.0f);
    cam.setDistance(100.0f);
}

//--------------------------------------------------------------
void ofApp::update(){
    float time = ofGetElapsedTimef();
    
    for(int i=0; i<LIGHT_NUM; i++) {
        ofVec3f vel = light[i].getVelocity();
        float r = light[i].getRadius();
        light[i].setPosition(sin(time * vel.x) * r,cos(time * vel.y) * r, sin(time * vel.z) * r);
    }
    
    cam.setPosition(sin(time * .3) * R*2, sin(time * .3)*R, cos(time * .3) * R*2);
    cam.lookAt(ofVec3f(0, 0, 0));
    
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofEnableDepthTest();
    glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    cam.begin();
    ofMatrix4x4 viewMatrix = ofGetCurrentViewMatrix();
    cam.end();
    
    gBufferShader.begin();
    gBufferShader.setUniformMatrix4f("view", viewMatrix);
    gBufferShader.setUniformMatrix4f("projection", cam.getProjectionMatrix());
    gBufferShader.setUniformTexture("normalMap", normalMap, 0);
    gBufferShader.setUniform1i("isBump", 1);
    for(int i=0; i<NUM; i++) {
        ofMatrix4x4 modelMatrix;
        modelMatrix.translate(pos[i]);
        gBufferShader.setUniformMatrix4f("model", modelMatrix);
        
        vboMesh.draw();
    }
    ofMatrix4x4 modelMatrix;
    modelMatrix.translate(0, 200, 0);
    modelMatrix.rotate(90,1,0,0);
    gBufferShader.setUniform1i("isBump", 0);
    gBufferShader.setUniformMatrix4f("model", modelMatrix);
    gBufferShader.setUniform3f("mColor", 1.0, 1.0, 1.0);
    floor.draw();
    
    for(int i=0; i<LIGHT_NUM; i++) {
        ofMatrix4x4 modelMatrix;
        modelMatrix.translate(light[i].getPosition());
        gBufferShader.setUniform1i("isBump", 0);
        gBufferShader.setUniformMatrix4f("model", modelMatrix);
        gBufferShader.setUniform3f("mColor", light[i].getColor());
        light[i].draw();
    }
    
    gBufferShader.end();
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    

    // rendering
    lightingShader.begin();
    lightingShader.setUniformTexture("gPosition", GL_TEXTURE_2D, gPosition, 0);
    lightingShader.setUniformTexture("gNormal", GL_TEXTURE_2D, gNormal, 1);
    lightingShader.setUniformTexture("gColorSpec", GL_TEXTURE_2D, gColorSpec, 2);
    lightingShader.setUniform3f("viewPos", cam.getPosition());
    lightingShader.setUniform1i("debugMode", debugMode);
    lightingShader.setUniform1f("lightDistance", lightDistance);
    lightingShader.setUniform1i("index", lightIndex);
    lightingShader.setUniform1f("lightAttenuation", lightAttenuation);
    lightingShader.setUniform1i("arrayDebug", arrayDebug);
    lightingShader.setUniform1f("disCoef", disCoef);
    for(int i=0; i<LIGHT_NUM; i++) {
        ofMatrix4x4 modelMatrix;
        modelMatrix.translate(light[i].getPosition());
        ofVec3f lightPos = light[i].getPosition() * viewMatrix;
        lightingShader.setUniform3fv("light["+to_string(i)+"].position", &lightPos[0], 1);
        lightingShader.setUniform3fv("light["+to_string(i)+"].color", &light[i].getColor()[0], 1);
    }
    quad.draw(OF_MESH_FILL);
    lightingShader.end();

    ofDrawBitmapString("debugMode: " + ofToString(debugMode), 10, 10);
//    glBindTexture(GL_TEXTURE_2D, gPosition);
//    quad.draw(OF_MESH_FILL);
    ofDisableDepthTest();
    gui.draw();
    
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
