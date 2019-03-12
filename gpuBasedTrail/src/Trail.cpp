//
//  Trail.cpp
//  gpuBasedTrail
//
//  Created by 木内舜司 on 2019/03/11.
//

#include "Trail.hpp"
//--------------------------------------------------------------
void Trail::init(int _num, int _length) {
    num = _num;
    length = _length;
}

//--------------------------------------------------------------
void Trail::createBuffers(int _num, int _length) {
    
    vector<float> pos(_length * _num * 4);
    for(int x = 0; x < _length; x++) {
        for(int y = 0; y < _num; y++) {
            int index = _length * y + x;
            pos[index * 3 + 0] = ofRandom(-1.0, 1.0);
            pos[index * 3 + 1] = ofRandom(-1.0, 1.0);
            pos[index * 3 + 2] = ofRandom(-1.0, 1.0);
            pos[index * 3 + 3] = 0.0;
        }
    }
    // pingPongBufferに初期値(位置)を書き込み
    posBuffer.allocate(_length, _num, GL_RGBA32F);
    posBuffer.src->getTexture().loadData(pos.data(), _length, _num, GL_RGBA);
    posBuffer.dst->getTexture().loadData(pos.data(), _length, _num, GL_RGBA);
    
    // パーティクルの速度設定(全て
    vector<float> vel(_length * _num * 4);
    for(int x = 0; x < _length; x++) {
        for(int y = 0; y < _num; y++) {
            int index = _length * y + x;
            vel[index * 3 + 0] = 0.0;
            vel[index * 3 + 1] = 0.0;
            vel[index * 3 + 2] = 0.0;
            vel[index * 3 + 3] = 0.0;
        }
    }
    // pingPongBufferに初期値(速度)を書き込み
    velBuffer.allocate(_length, _num, GL_RGBA32F);
    velBuffer.src->getTexture().loadData(vel.data(), _length, _num, GL_RGBA);
    velBuffer.dst->getTexture().loadData(vel.data(), _length, _num, GL_RGBA);
}

void Trail::debugTexture(int x, int y) {
    ofDrawBitmapString("pos", x, y - 5);
    posBuffer.src->draw(x, y, length, num * 0.1);
    ofDrawBitmapString("pos", x + length * 2, y - 5);
    velBuffer.src->draw(x + length * 2, y, length, num * 0.1);
}

//--------------------------------------------------------------
void Trail::draw(ofPolyRenderMode mode) {
    
    render.begin();
    render.setUniformTexture("posTex", posBuffer.src->getTexture(), 0);
    render.setUniform1i("numTrail", num);
    render.setUniform1i("trailLength", length);
    render.setUniform1f("trailWidth", width);
    render.setUniform1f("size", size);
    render.setUniform3f("camPos", camPos);
    render.setUniform1f("time", ofGetElapsedTimef());
    render.setUniform2f("texSize", (float)length, (float)num);
    mesh.drawInstanced(mode, num);
    render.end();
}

//--------------------------------------------------------------
void Trail::update() {
    // 速度ピンポン
    velBuffer.dst->begin();
    ofClear(0);
    updateVel.begin();
    // １フレーム前の速度情報
    updateVel.setUniformTexture("prevVelTex", velBuffer.src->getTexture(), 0);
    // 頂点情報
    updateVel.setUniformTexture("posTex", posBuffer.src->getTexture(), 1);
    // 各種Uniform変数
    updateVel.setUniform1f("time", ofGetElapsedTimef());
    updateVel.setUniform1f("timestep", 0.005f);
    
    // 更新された速度情報をテクスチャに書き込む
    velBuffer.src->draw(0, 0);
    
    updateVel.end();
    velBuffer.dst->end();
    
    velBuffer.swap();
    
    // ========== 頂点ピンポン =========
    // 頂点更新
    posBuffer.dst->begin();
    ofClear(0);
    updatePos.begin();
    updatePos.setUniform1f("speed", speed);
    updatePos.setUniformTexture("posTex", posBuffer.src->getTexture(), 0);
    updatePos.setUniformTexture("velTex", velBuffer.src->getTexture(), 1);
    
    posBuffer.src->draw(0, 0);
    
    updatePos.end();
    posBuffer.dst->end();
    
    posBuffer.swap();
}
