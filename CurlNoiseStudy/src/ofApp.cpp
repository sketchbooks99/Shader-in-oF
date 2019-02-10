#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    soundStream.printDeviceList();
    int bufferSize = 256;
    
    left.assign(bufferSize, 0.0);
    right.assign(bufferSize, 0.0);
    
    bufferCounter = 0;
    smoothedVol = 0;
    scaledVol = 0;
    
    ofSoundStreamSettings settings;
    
    auto device = soundStream.getMatchingDevices("default");
    if(!device.empty()) {
        settings.setInDevice(device[2]);
    }
    
    settings.setInListener(this);
    settings.sampleRate = 44100;
    settings.numInputChannels = 2;
    settings.numOutputChannels = 2;
    settings.bufferSize = bufferSize;
    soundStream.setup(settings);
    
    soundStream.start();
    
    
    ofSetVerticalSync(true);
//    ofBackground(0);
    ofSetFrameRate(30);
    ofDisableAlphaBlending();
    ofSetBackgroundAuto(false);
    
    
    cam.setupPerspective();
    
    numParticles = 600 * 600;
    
    // アップデート用シェーダ
    updatePos.load("shader/passthru.vert", "shader/posUpdate.frag");
    // レンダリング用シェーダ
    render.load("shader/render.vert", "shader/render.frag");
    // ブラー用シェーダ
    blur.load("shader/passthru.vert", "shader/blur.frag");
    // 最終描画用シェーダ
    result.load("shader/passthru.vert", "shader/result.frag");
    renderFbo.allocate(ofGetWidth(), ofGetHeight());
    onepassFbo.allocate(ofGetWidth(), ofGetHeight());
    twopassFbo.allocate(ofGetWidth(), ofGetHeight());
    
    texRes = (int)sqrt((float)numParticles);
    numParticles = texRes * texRes;
    
    // particleの設定
    particles.setMode(OF_PRIMITIVE_POINTS);
    for(int x = 0; x < texRes; x++) {
        for(int y = 0; y < texRes; y++) {
            int index = texRes * y + x;
            if(index < numParticles) {
                particles.addVertex(ofVec3f(0, 0, 0));
                particles.addTexCoord(ofVec2f(x, y));
                particles.addColor(ofFloatColor(1.0, 1.0, 1.0, 0.5));
            }
        }
    }
    
    // パーティクルの座標・加速度の保存用FBO
    // RGBA32Fの形式で2つのColorBufferを用意
    
    posPingPong.allocate(texRes, texRes, GL_RGBA32F, 2);
    
    // パーティクルの位置と経過時間の初期設定
    float * posAndAge = new float[texRes * texRes * 4];
    for(int x = 0; x < texRes; x++) {
        for(int y = 0; y < texRes; y++) {
            int i = texRes * y + x;
            posAndAge[i*4+0] = ofRandom(-1.0, 1.0);
            posAndAge[i*4+1] = ofRandom(-1.0, 1.0);
            posAndAge[i*4+2] = ofRandom(-1.0, 1.0);
            posAndAge[i*4+3] = 0;
        }
    }
    // pingPongBufferに初期値を書き込み
    
    posPingPong.src->getTexture(0).loadData(posAndAge, texRes, texRes, GL_RGBA);
    delete [] posAndAge;
    
    // パーティクルの速度と生存期間の設定
    float * velAndMax = new float[texRes * texRes * 4];
    for(int x = 0; x < texRes; x++) {
        for(int y = 0; y < texRes; y++) {
            int i = texRes * y + x;
            velAndMax[i*4+0] = ofRandom(-1, 1);
            velAndMax[i*4+1] = ofRandom(-1, 1);
            velAndMax[i*4+2] = ofRandom(-1, 1);
            // velAndMax[i*4+3] = ofRandom(1, 150);
            velAndMax[i*4+3] = 60.0;
        }
    }
    
    // pingPongBufferに書き込み
    posPingPong.src->getTexture(1).loadData(velAndMax, texRes, texRes, GL_RGBA);
    delete [] velAndMax;
}

//--------------------------------------------------------------
void ofApp::update(){
    if(smoothedVol > 0.05)
        scaledVol = ofMap(smoothedVol, 0.0, 0.1, 200.0, 500.0);
    else
        scaledVol = 200.0;
    cout << smoothedVol << endl;
    
    if(count > 60) count = 0;
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
    
    float time = ofGetElapsedTimef();
    
    // パーティクルの発生位置を更新
    prevEmitterPos = emitterPos;
    emitterPos = 150 * ofVec3f(ofSignedNoise(time, 0, 0), ofSignedNoise(0, time, 0), ofSignedNoise(0, 0, time));
    
    // パーティクルの位置を計算
    posPingPong.dst->begin();
    
    // 複数バッファの書き出しを有効化
    posPingPong.dst->activateAllDrawBuffers();
    ofClear(0);
    updatePos.begin();
    
    // パーティクルの位置と経過時間
    updatePos.setUniformTexture("posAndAgeTex", posPingPong.src->getTexture(0), 0);
    
    // パーティクルの速度と生存期間
    updatePos.setUniformTexture("velAndMaxTex", posPingPong.src->getTexture(1), 1);
    // パーティクルの色
    updatePos.setUniform1f("time", time);
    updatePos.setUniform1f("timestep", 0.1);
    updatePos.setUniform1f("scale", 0.01);
    updatePos.setUniform3f("emitterPos", emitterPos);
    updatePos.setUniform3f("prevEmitterPos", prevEmitterPos);
    updatePos.setUniform1f("radius", scaledVol);
    updatePos.setUniform1i("mode", mode);
    posPingPong.src->draw(0, 0);
    updatePos.end();
    posPingPong.dst->end();
    posPingPong.swap();
    
    
    // レンダリング
    renderFbo.begin();
    ofPushStyle();
    ofEnableAlphaBlending();
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    ofBackground(0, 0, 0, 50);
    
    float radius = 700.0;
        cam.setPosition(sin(time)*cos(TWO_PI)*radius, sin(time)*sin(TWO_PI)*radius, cos(time)*radius);
        cam.lookAt(glm::vec3(0.0, 0.0, 0.0));
    
    cam.begin();
    
    ofPushMatrix();
    
    render.begin();
    
    render.setUniformTexture("posAndAgeTex", posPingPong.src->getTexture(0), 0);
    render.setUniform1f("time", ofGetElapsedTimef());
    particles.draw();
    
    render.end();
    
    ofPopMatrix();
    
    cam.end();
    ofPopStyle();
    renderFbo.end();
    
    // horizontal
    onepassFbo.begin();
    blur.begin();
    blur.setUniformTexture("tex", renderFbo.getTexture(), 0);
    blur.setUniform1f("size", 5.0);
    blur.setUniform2f("direction", 5.0, 0.0);
    
    renderFbo.draw(0, 0);
    
    blur.end();
    
    onepassFbo.end();
    
    // vertical
    twopassFbo.begin();
    blur.begin();
    blur.setUniformTexture("tex", onepassFbo.getTexture(), 0);
    blur.setUniform1f("size", 5.0);
    blur.setUniform2f("direction", 0.0, 5.0);
    
    onepassFbo.draw(0, 0);
    
    blur.end();
    
    twopassFbo.end();
    
    count++;
}

//--------------------------------------------------------------
void ofApp::draw(){
    result.begin();
    result.setUniformTexture("renderTex", renderFbo.getTexture(), 0);
    result.setUniformTexture("blurTex", twopassFbo.getTexture(), 1);
    renderFbo.draw(0, 0);
    result.end();
    
}

//--------------------------------------------------------------
void ofApp::audioIn(ofSoundBuffer & input) {
    float curVol = 0.0;
    
    int numCounted = 0;
    
    for(size_t i=0; i<input.getNumFrames(); i++) {
        left[i] = input[i*2] * 0.5;
        right[i] = input[i*2+1] * 0.5;
        
        curVol += left[i] * left[i];
        curVol += right[i] * right[i];
        numCounted += 2;
    }
    
    curVol /= (float)numCounted;
    
    curVol = sqrt(curVol);
    
    smoothedVol *= 0.93;
    smoothedVol += 0.07 * curVol;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    mode = (int)ofRandom(0, 5);
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
