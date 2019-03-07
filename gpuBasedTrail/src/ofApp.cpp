#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(0);
    ofSetVerticalSync(true);
    
    numTrail = 10000;
    trailLength = 10;
    updatePos.load("shader/passthru.vert", "shader/posUpdate.frag");
    updateVel.load("shader/passthru.vert", "shader/velUpdate.frag");
    createTrail.load("shader/createTrail.vert", "shader/createTrail.frag", "shader/createTrail.geom");
    
    // trailを描く用のメッシュ
    int numVertices = numTrail * trailLength;
    for(int x = 0; x < trailLength; x++) {
        for(int y = 0; y < numTrail; y++) {
            int index = trailLength * y + x;
            if(index < numVertices) {
                trailVertices.addVertex(ofVec3f(0, 0, 0));
                trailVertices.addTexCoord(ofVec2f(x, y));
                trailVertices.addColor(ofFloatColor(1,1,1,1));
            }
        }
    }
    
    // パーティクルの座標・速度の保存用FBO
    // RGB32F形式で2つのColorBufferを用意
    posPingPong.allocate(trailLength, numTrail, GL_RGB32F, 2);
    
    // パーティクルの位置設定
    float * pos = new float[trailLength * numTrail * 3];
    for(int x = 0; x < trailLength; x++) {
        for(int y = 0; y < numTrail; y++) {
            int index = trailLength * y + x;
            pos[index * 3 + 0] = ofRandom(-1.0, 1.0);
            pos[index * 3 + 1] = ofRandom(-1.0, 1.0);
            pos[index * 3 + 2] = ofRandom(-1.0, 1.0);
        }
    }
    // pingPongBufferに初期値(位置)を書き込み
    posPingPong.src->getTexture(0).loadData(pos, trailLength, numTrail, GL_RGB);
    delete[] pos;
    
    // パーティクルの速度設定(全て0)
    float * vel = new float[trailLength * numTrail * 3];
    for(int x = 0; x < trailLength; x++) {
        for(int y = 0; y < trailLength; y++) {
            int index = trailLength * y + x;
            vel[index * 3 + 0] = 0.0;
            vel[index * 3 + 1] = 0.0;
            vel[index * 3 + 2] = 0.0;
        }
    }
    // pingPongBufferに初期値(速度)を書き込み
    posPingPong.src->getTexture(1).loadData(vel, trailLength, numTrail, GL_RGB);
    delete[] vel;
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

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
