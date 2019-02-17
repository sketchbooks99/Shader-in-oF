#pragma once

#include "ofMain.h"
#include "ofxGui.h"

#define NUM 1
#define LIGHT_NUM 40
#define R 40
#define BOX_SIZE 20

class PointLight {
public:
    PointLight() {
        sphere = ofSpherePrimitive(8, 8);
        velocity = ofVec3f(ofRandom(-0.5,0.5),ofRandom(-0.5,0.5),ofRandom(-0.5,0.5));
    }
    
    // color
    void setColor(float r, float g, float b){
        color = ofVec3f(r, g, b);
    }
    void setColor(float _color) {
        color = ofVec3f(_color, _color, _color);
    }
    void setGray(float _gray) {
        gray = _gray;
    }
    void setColor(ofVec3f _color) {
        color = _color;
    }
    ofVec3f getColor() const { return color; }
    float getGray() const { return gray; }
    
    // position
    void setPosition(float x, float y, float z){
        position = ofVec3f(x, y, z);
    }
    void setPosition(ofVec3f _position){
        position = _position;
    }
    ofVec3f getPosition() const { return position; }
    
    void setVelocity(float x, float y, float z){
        velocity = ofVec3f(x, y, z);
    }
    ofVec3f getVelocity() const { return velocity; }
    
    void setRadius(float _radius) {
        radius = _radius;
    }
    float getRadius() const { return radius; }
    
    void draw() {
//        ofSetColor(color.x, color.y, color.z);
        sphere.draw();
    }
private:
    ofVec3f position;
    ofVec3f color;
    float gray;
    ofVec3f velocity;
    float radius;
    
    ofSpherePrimitive sphere;
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
    void createGBuffer();
    void createRenderBuffer();
		
    ofShader gBufferShader, lightingShader, floorShader, lightShader, renderShader;
    
    unsigned int gBuffer, renderBuffer;
    unsigned int gPosition, gNormal, gColorSpec, gRender;
    
    ofVboMesh quad, vboMesh, floor;
    ofTexture normalMap;
    
    ofFbo gFbo, renderFbo, lightFbo;
    
    PointLight light[LIGHT_NUM];

    ofVec3f pos[NUM];
    
    ofEasyCam cam;
    
    ofxPanel gui;
    ofParameter<int> debugMode;
    ofParameter<float> lightDistance;   // lightを適用する距離
    ofParameter<int> lightIndex;        // GLSL上での配列のdebug用
    ofParameter<float> lightAttenuation; // 距離減衰の減衰係数
    ofParameter<float> disCoef;
    ofxToggle arrayDebug, isGray;
};
