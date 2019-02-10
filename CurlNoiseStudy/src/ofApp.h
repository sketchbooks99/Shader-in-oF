#pragma once

#include "ofMain.h"

struct pingPongBuffer {
public:
    void allocate(int _width, int _height, int _internalformat = GL_RGBA, int _numColorBuffers = 1) {
        // Allocate
        ofFbo::Settings fboSettings;
        fboSettings.width = _width;
        fboSettings.height = _height;
        fboSettings.numColorbuffers = _numColorBuffers;
        fboSettings.useDepth = false;
        fboSettings.internalformat = _internalformat;
        fboSettings.wrapModeHorizontal = GL_CLAMP_TO_EDGE;
        fboSettings.wrapModeVertical = GL_CLAMP_TO_EDGE;
        fboSettings.minFilter = GL_NEAREST;
        fboSettings.maxFilter = GL_NEAREST;
        
        for(int i = 0; i < 2; i++) {
            FBOs[i].allocate(fboSettings);
        }
        
        src = &FBOs[0];
        dst = &FBOs[1];
        
        clear();
    }
    
    void swap() {
        std::swap(src, dst);
    }
    
    void clear() {
        for(int i = 0; i < 2; i++) {
            FBOs[i].begin();
            ofClear(0, 255);
            FBOs[i].end();
        }
    }
    
    ofFbo& operator[](int n) { return FBOs[n]; }
    ofFbo *src; // Source      -> Ping
    ofFbo *dst; // Destination -> Pong
    
private:
    ofFbo FBOs[2]; // Real addresses of ping/pong
};

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    void audioIn(ofSoundBuffer & input);
    
    ofVboMesh particles;
    ofShader render, updatePos, blur, result;
    ofEasyCam cam;
    
    pingPongBuffer posPingPong;
    
    ofVec3f emitterPos, prevEmitterPos;
    int numParticles, texRes;
    
    ofFbo renderFbo, onepassFbo, twopassFbo;
    
    int mode = 0, count = 0;
    bool isPressed;
    
    
    // sound
    vector<float> left;
    vector<float> right;
    
    int bufferCounter;
    
    float smoothedVol;
    float scaledVol;
    
    ofSoundStream soundStream;
};
