#pragma once

#include "ofMain.h"
#include "ofxGui.h"

#define NUM 30
#define LIGHT_NUM 10
#define R 50

class PointLight {
public:
    PointLight() {
        sphere = ofSpherePrimitive(2, 8);
        velocity = ofVec3f(ofRandom(-0.5,0.5),ofRandom(-0.5,0.5),ofRandom(-0.5,0.5));
    }
    
    // color
    void setColor(float r, float g, float b){
        color = ofVec3f(r, g, b);
    }
    ofVec3f getColor() const { return color; };
    
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
        ofSetColor(color.x, color.y, color.z);
        sphere.draw();
    }
private:
    ofVec3f position;
    ofVec3f color;
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
		
    ofShader gBufferShader, lightingShader;
    
    unsigned int gBuffer;
    unsigned int gPosition, gNormal, gColorSpec;
    
    ofVboMesh quad, vboMesh, floor;
    ofTexture normalMap;
    
    PointLight light[LIGHT_NUM];

    ofVec3f pos[NUM];
    
    ofEasyCam cam;
    
    ofxPanel gui;
    ofParameter<int> debugMode;
    ofParameter<float> lightDistance;   // lightを適用する距離
    ofParameter<int> lightIndex;        // GLSL上での配列のdebug用
    ofParameter<float> lightAttenuation; // 距離減衰の減衰係数
    ofParameter<float> disCoef;
    ofxToggle lightDebug, arrayDebug;
};
