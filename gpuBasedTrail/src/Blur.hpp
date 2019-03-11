//
//  PostEffect.hpp
//  gpuBasedTrail
//
//  Created by 木内舜司 on 2019/03/11.
//

#ifndef Blur_hpp
#define Blur_hpp

#include "ofMain.h"
#define STRINGIFY(A) string("#version 150\n")+#A

class Blur {
public:
    
    void allocate(int _width, int _height) {
        target.allocate(_width, _height);
        onePassFbo.allocate(_width, _height);
        result.allocate(_width, _height);
    }
    
    void begin() {
        target.begin();
        ofClear(0);
        ofBackground(100);
    }
    
    void end() {
        target.end();
        
        // horizontal
        onePassFbo.begin();
        
        blurShader.begin();
        blurShader.setUniform1f("blurSize", size);
        blurShader.setUniformTexture("tex", target.getTexture(), 0);
        blurShader.setUniform2f("direction", 1.0, 0.0);
        
        target.draw(0, 0);
        
        blurShader.end();
        onePassFbo.end();
        
        // vertical
        result.begin();
        
        blurShader.begin();
        blurShader.setUniform1f("blurSize", size);
        blurShader.setUniformTexture("tex", onePassFbo.getTexture(), 0);
        blurShader.setUniform2f("direction", 0.0, 1.0);
        
        onePassFbo.draw(0, 0);
        
        blurShader.end();
        result.end();
    }
    
    void draw(int x, int y) {
        result.draw(x, y);
    }
    void draw(int x, int y, int w, int h) {
        result.draw(x, y, w, h);
    }
    
    void setupShaders() {
        blurShader.load("shader/passthru.vert", "shader/blur.frag");
        bloomShader.load("shader/passthru.vert", "shader/bloom.frag");
    }
    
    void bloomDraw(int x, int y) {
        bloomShader.begin();
        bloomShader.setUniformTexture("origin", target.getTexture(), 0);
        bloomShader.setUniformTexture("result", result.getTexture(), 1);
        bloomShader.setUniform1f("strength", strength);
        
        target.draw(x, y);
        
        bloomShader.end();
    }
    void bloomDraw(int x, int y, int w, int h) {
        bloomShader.begin();
        bloomShader.setUniformTexture("origin", target.getTexture(), 0);
        bloomShader.setUniformTexture("result", result.getTexture(), 1);
        bloomShader.setUniform1f("strength", strength);
        
        target.draw(x, y, w, h);
        
        bloomShader.end();
    }
    
    void debugFbo() {
        int w = target.getWidth() / 2;
        int h = target.getHeight() / 2;
        target.draw(0, 0, w, h);
        onePassFbo.draw(w, 0, w, h);
        result.draw(0, h, w, h);
        bloomDraw(w, h, w, h);
    }
    
    ofFbo getResult() { return result; }
    ofFbo getTarget() { return target; }
    
    void setSize(float _size) { size = _size; }
    
    void setStrength(float _strength) { strength = _strength; }
    
private:
    float size, strength;
    ofShader blurShader, bloomShader;
    ofFbo target, onePassFbo, result;
};



#endif /* PostEffect_hpp */
