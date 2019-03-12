#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);
    ofSetFrameRate(30);
    ofEnableAlphaBlending();
    ofEnableSmoothing();
    
    numTrail = 8192;
    trailLength = 10;
    
    trail.init(numTrail, trailLength);
    trail.createMesh(trail.getLength());
    trail.createBuffers(trail.getNum(), trail.getLength());
    trail.loadVelocityShader("shader/passthru.vert", "shader/velUpdate.frag");
    trail.loadPositionShader("shader/passthru.vert", "shader/posUpdate.frag");
    trail.loadRenderShader("shader/createTrail.vert", "shader/createTrail.frag", "shader/createTrail.geom");
    
    light.setPosition(500, 500, -500);

    mesh = ofSpherePrimitive(300, 32).getMesh();
    for(int i=0; i<mesh.getVertices().size(); i++) {
        mesh.addColor(ofFloatColor(0.5,0.5, 0.5, 1.0));
    }
    
    blur.allocate(ofGetWidth(), ofGetHeight());
    blur.setupShaders();
    blur.setSize(5.0);
    blur.setStrength(1.0);
    
    gui.setup();
    gui.setPosition(10, 10);
    gui.add(trailWidth.set("Width", 7.0, 0.1, 10.0));
    gui.add(speed.set("Speed", 2.0, 0.1, 5.0));
    gui.add(size.set("Size",100.0, 50.0, 200.0));
    gui.add(blurSize.set("BlurSize", 10.0, 1.0, 20.0));
    gui.add(blurStrength.set("BlurStrength", 2.5, 0.1, 5.0));
    gui.add(camRadius.set("CamRadius", 1500.0, 500.0, 3000.0));
    
    // sound
//    soundStream.printDeviceList();
//    int bufferSize = 256;
//
//    left.assign(bufferSize, 0.0);
//    right.assign(bufferSize, 0.0);
//
//    bufferCounter = 0;
//    smoothedVol = 0;
//    scaledVol = 0;
//
//    ofSoundStreamSettings settings;
//
//    auto device = soundStream.getMatchingDevices("default");
//    if(!device.empty()) {
//        settings.setInDevice(device[2]);
//    }
//
//    settings.setInListener(this);
//    settings.sampleRate = 44100;
//    settings.numInputChannels = 2;
//    settings.numOutputChannels = 2;
//    settings.bufferSize = bufferSize;
//    soundStream.setup(settings);
//
//    soundStream.start();
}

//--------------------------------------------------------------
void ofApp::update(){
    
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
    
    trail.setWidth(trailWidth);
    trail.update();
    
    blur.setSize(blurSize);
    blur.setStrength(blurStrength);
    
    float time = ofGetElapsedTimef();
    cam.setPosition(sin(time * 0.5) * camRadius, sin(time * 0.5) * 500.0, cos(time * 0.5) * camRadius);
    cam.lookAt(ofVec3f(0, 0, 0));
    
    blur.begin();
    
    ofEnableDepthTest();
    
    cam.begin();
    
    trail.setUniforms(cam.getGlobalPosition(), speed, size);
    trail.draw(OF_MESH_FILL);
    
    mesh.draw();
    
    cam.end();
    
    ofDisableDepthTest();
    
    blur.end();
}

//--------------------------------------------------------------
void ofApp::draw(){
    blur.bloomDraw(0, 0);
    
    if(isGui) {
        gui.draw();
    }
}

//--------------------------------------------------------------
void ofApp::audioIn(ofSoundBuffer & input) {
//    float curVol = 0.0;
//
//    int numCounted = 0;
//
//    for(size_t i=0; i<input.getNumFrames(); i++) {
//        left[i] = input[i*2] * 0.5;
//        right[i] = input[i*2+1] * 0.5;
//
//        curVol += left[i] * left[i];
//        curVol += right[i] * right[i];
//        numCounted += 2;
//    }
//
//    curVol /= (float)numCounted;
//
//    curVol = sqrt(curVol);
//
//    smoothedVol *= 0.93;
//    smoothedVol += 0.07 * curVol;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch(key) {
        case 'g':
            isGui = !isGui;
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
