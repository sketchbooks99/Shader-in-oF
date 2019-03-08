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
    posPingPong.allocate(trailLength, numTrail, GL_RGB32F);
    posPingPong.src->getTexture().loadData(pos, trailLength, numTrail, GL_RGB);
    posPingPong.dst->getTexture().loadData(pos, trailLength, numTrail, GL_RGB);
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
    velPingPong.allocate(trailLength, numTrail, GL_RGB32F);
    velPingPong.src->getTexture().loadData(vel, trailLength, numTrail, GL_RGB);
    velPingPong.dst->getTexture().loadData(vel, trailLength, numTrail, GL_RGB);
    delete[] vel;
}

//--------------------------------------------------------------
void ofApp::update(){
    float time = ofGetElapsedTimef();
    
    // 速度ピンポン
    // 速度更新
    velPingPong.dst->begin();
    ofClear(0);
    updateVel.begin();
    // １フレーム前の速度情報
    updateVel.setUniformTexture("prevVelTex", velPingPong.src->getTexture(), 0);
    // 頂点情報
    updateVel.setUniformTexture("posTex", posPingPong.src->getTexture(), 1);
    // 各種Uniform変数
    updateVel.setUniform2i("resolution", trailLength, numTrail);
    updateVel.setUniform2f("screen", (float)ofGetWidth(), (float)ofGetHeight());
    updateVel.setUniform1f("time", time);
    updateVel.setUniform1f("timestep", 0.005f);
    
    // 更新された速度情報をテクスチャに書き込む
    velPingPong.src->draw(0, 0);
    
    updateVel.end();
    velPingPong.dst->end();
    
    velPingPong.swap();
    
    // 頂点ピンポン
    // 頂点更新
    posPingPong.dst->begin();
    ofClear(0);
    updatePos.begin();
    updatePos.setUniformTexture("prevPosTex", posPingPong.src->getTexture(), 0);
    updatePos.setUniformTexture("velTex", posPingPong.src->getTexture(), 1);
    updatePos.setUniform1f("timestep", 0.005f);
    
    posPingPong.src->draw(0, 0);
    
    updatePos.end();
    posPingPong.dst->end();
    
    posPingPong.swap();
}

//--------------------------------------------------------------
void ofApp::draw(){
    // trail rendering using posTex
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
