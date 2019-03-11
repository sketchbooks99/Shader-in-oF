//
//  Trail.hpp
//  gpuBasedTrail
//
//  Created by 木内舜司 on 2019/03/11.
//

#ifndef Trail_hpp
#define Trail_hpp

#include "ofMain.h"

struct pingPongBuffer {
public:
    void allocate(int _width, int _height, int _internalformat = GL_RGBA) {
        
        for(int i = 0; i < 2; i++) {
            FBOs[i].allocate(_width, _height, _internalformat);
            FBOs[i].getTexture().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
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
    ofFbo FBOs[2]; // real addresses of ping/pong
};

class Trail {
public:
    void init(int _num, int _length);
    void createBuffers(int _num, int _length);
    void draw(ofPolyRenderMode mode = OF_MESH_FILL);
    void debugTexture(int x, int y);
    void update();
    
    void loadVelocityShader(std::string frag, std::string vert) {
        updateVel.load(frag, vert);
    }
    
    void loadPositionShader(std::string frag, std::string vert) {
        updatePos.load(frag, vert);
    }
    
    void loadRenderShader(std::string frag, std::string vert) {
        render.load(frag, vert);
    }
    
    void loadRenderShader(std::string frag, std::string vert, std::string geom) {
        render.load(frag, vert, geom);
    }
    
    int getLength() { return length; }
    int getNum() { return num; }
    
    void setWidth(float _width) { width = _width; }
    float getWidth() { return width; }
    
private:
    float num, length, width;
    ofFloatColor color;
    pingPongBuffer velBuffer, posBuffer;
    ofVboMesh mesh;
    ofShader render, updateVel, updatePos;
};

#endif /* Trail_hpp */
