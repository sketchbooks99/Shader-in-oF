#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(0);
    ofSetVerticalSync(true);
    
    gBufferShader.load("shader/gBuffer.vert","shader/gBuffer.frag","shader/gBuffer.geom");
    lightingShader.load("shader/lighting.vert","shader/lighting.frag");
    lightShader.load("shader/light.vert", "shader/light.frag");
    
    ofDisableArbTex();
    ofLoadImage(normalMap, "image/normal.jpg");
    
    pos[0] = ofVec3f(0, -R * 3, 0);
    
//    for(int x=-2; x<=2; x++) {
//        for(int y=-2; y<=2; y++) {
//            for(int z=-2; z<=2; z++) {
//                int index = 25 * (x+2) + 5 * (y+2) + (z+2);
//                cout << index << endl;
//                float xPos = R * x;
//                float yPos = R * y - (R*2 + 10);
//                float zPos = R * z;
//
//                pos[index] = ofVec3f(xPos,yPos,zPos);
//            }
//        }
//    }
//
    for(int i=0; i<LIGHT_NUM; i++) {
        ofVec3f lightPos = ofVec3f(ofRandom(R*3), ofRandom(R*3), ofRandom(R*3));
        light[i].setPosition(lightPos);
        light[i].setColor(ofRandom(1.0), ofRandom(1.0), ofRandom(1.0));
        light[i].setGray(ofRandom(1.0));
        light[i].setRadius(lightPos.length());
    }
    
//    vboMesh = ofBoxPrimitive(BOX_SIZE, BOX_SIZE, BOX_SIZE).getMesh();
    vboMesh = ofIcoSpherePrimitive(R*2, 2).getMesh();
    floor = ofPlanePrimitive(1000, 1000, 20, 20).getMesh();
    
    quad.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
    quad.addVertex(ofVec3f(ofGetWidth(), ofGetHeight(), 0.0));
    quad.addTexCoord(ofVec2f(1.0f, 1.0f));
    quad.addVertex(ofVec3f(ofGetWidth(), 0.0, 0.0));
    quad.addTexCoord(ofVec2f(1.0f, 0.0f));
    quad.addVertex(ofVec3f(0.0, 0.0, 0.0));
    quad.addTexCoord(ofVec2f(0.0f, 0.0f));
    quad.addVertex(ofVec3f(0.0, ofGetHeight(), 0.0));
    quad.addTexCoord(ofVec2f(0.0f, 1.0f));
    
    gui.setup();
    gui.setPosition(10, 10);
    gui.add(debugMode.set("DebugMode", 0, 0, 3));
    gui.add(arrayDebug.setup("Array Debug", false));
    gui.add(isGray.setup("Is Gray", false));
    gui.add(lightIndex.set("Light Index", 0, 0, LIGHT_NUM - 1));
    gui.add(lightAttenuation.set("Attenuation", 0.005, 0.0, 0.15));
    gui.add(disCoef.set("Distance Coefficient", 0.2, 0.0, 1.0));
    
    lightFbo.allocate(ofGetWidth(), ofGetHeight());
    
    
    // fbo setting
    ofFbo::Settings gSettings;
    gSettings.textureTarget = GL_TEXTURE_2D;
    gSettings.width = ofGetWidth();
    gSettings.height = ofGetHeight();
    gSettings.internalformat = GL_RGB32F;
    gSettings.useDepth = true;
    gSettings.useStencil = true;
    gSettings.depthStencilAsTexture = true;
    gSettings.depthStencilInternalFormat = GL_DEPTH_COMPONENT; // depth
    gSettings.maxFilter = GL_NEAREST;
    gSettings.minFilter = GL_NEAREST;
    gSettings.wrapModeVertical = GL_CLAMP_TO_EDGE;
    gSettings.wrapModeHorizontal = GL_CLAMP_TO_EDGE;
    gFbo.allocate(gSettings);
    
    gFbo.createAndAttachTexture(GL_RGB32F, 1); // position
    gFbo.createAndAttachTexture(GL_RGB32F, 2); // normal
    gFbo.createAndAttachTexture(GL_RGB32F, 3); // color
    gFbo.checkStatus();
    
    ofFbo::Settings rSettings; // render fbo settings
    rSettings.textureTarget = GL_TEXTURE_2D;
    rSettings.width = ofGetWidth();
    rSettings.height = ofGetHeight();
    rSettings.useDepth = true;
    rSettings.useStencil = true;
    rSettings.internalformat = GL_RGB32F;
    rSettings.depthStencilAsTexture = true;
    rSettings.depthStencilInternalFormat = GL_DEPTH_COMPONENT;
    rSettings.maxFilter = GL_NEAREST;
    rSettings.minFilter = GL_NEAREST;

    rSettings.wrapModeHorizontal = GL_CLAMP_TO_EDGE;
    rSettings.wrapModeVertical = GL_CLAMP_TO_EDGE;
    renderFbo.allocate(rSettings);
    
    renderFbo.createAndAttachTexture(GL_RGB32F, 1);
}

//--------------------------------------------------------------
void ofApp::update(){
    float time = ofGetElapsedTimef();
    int timeInt = (int)time;
    float strength = 1.0;
    
    for(int i=0; i<LIGHT_NUM; i++) {
        ofVec3f vel = light[i].getVelocity();
        float r = light[i].getRadius();
        light[i].setPosition(sin(time * vel.x) * r * 2,cos(time * vel.y) * r - R * 3, sin(time * vel.z) * r * 2);
    }
    
    cam.setPosition(sin(time * 0.5 * strength) * R * 6, -R * 4, cos(time * .5 * strength) * R * 6);
    cam.lookAt(ofVec3f(0, -R * 2, 0));
    
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void ofApp::draw(){
    gFbo.begin();
    gFbo.activateAllDrawBuffers();
    ofEnableDepthTest(); // 深度テストを有効に
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    cam.begin();
    ofMatrix4x4 viewMatrix = ofGetCurrentViewMatrix();
    cam.end();
    
    // gBufferに描画結果を格納する
    gBufferShader.begin();
    ofMatrix4x4 projection = cam.getProjectionMatrix();
    gBufferShader.setUniformMatrix4f("view", viewMatrix);
    gBufferShader.setUniformMatrix4f("projection", projection);
    gBufferShader.setUniformTexture("normalMap", normalMap, 0);
    gBufferShader.setUniform1f("time", ofGetElapsedTimef());
    gBufferShader.setUniform1i("isBump", 1);
    for(int i=0; i<NUM; i++) {
        ofMatrix4x4 modelMatrix;
        modelMatrix.translate(pos[i]);
        gBufferShader.setUniformMatrix4f("model", modelMatrix);
        
        vboMesh.draw();
    }
    ofMatrix4x4 modelMatrix;
    modelMatrix.rotate(90,1,0,0);
    gBufferShader.setUniform1i("isBump", 0);
    gBufferShader.setUniformMatrix4f("model", modelMatrix);
    gBufferShader.setUniform3f("mColor", 1.0, 1.0, 1.0);
    
    floor.draw();
    
    gBufferShader.end();
    
    gFbo.end();
    
    // ライティング処理部分
    renderFbo.begin();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    lightingShader.begin();
    ofMatrix4x4 model;
    model.translate(0, 0, 0);
    lightingShader.setUniformTexture("gPosition", gFbo.getTexture(0), 0);
    lightingShader.setUniformTexture("gNormal", gFbo.getTexture(1), 1);
    lightingShader.setUniformTexture("gColorSpec", gFbo.getTexture(2), 2);
    lightingShader.setUniform3f("viewPos", cam.getPosition());
    lightingShader.setUniform1i("debugMode", debugMode);
    lightingShader.setUniform1f("lightDistance", lightDistance);
    lightingShader.setUniform1i("index", lightIndex);
    lightingShader.setUniform1f("lightAttenuation", lightAttenuation);
    lightingShader.setUniform1i("arrayDebug", arrayDebug);
    lightingShader.setUniform1f("disCoef", disCoef);
    for(int i=0; i<LIGHT_NUM; i++) {
        ofMatrix4x4 modelMatrix;
        modelMatrix.translate(light[i].getPosition());
        ofVec3f lightPos = light[i].getPosition() * viewMatrix;
        lightingShader.setUniform3fv("light["+to_string(i)+"].position", &lightPos[0], 1);
        if(isGray){
            ofVec3f grayColor = ofVec3f(light[i].getGray());
            lightingShader.setUniform3fv("light["+to_string(i)+"].color", &grayColor[0], 1);
        } else {
            lightingShader.setUniform3fv("light["+to_string(i)+"].color", &light[i].getColor()[0], 1);
        }
    }
    quad.draw(OF_MESH_FILL);
    lightingShader.end();
    renderFbo.end();
    
    // pointLight描画部分
    glBindFramebuffer(GL_READ_FRAMEBUFFER, gFbo.getId());
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, renderFbo.getIdDrawBuffer()); // write to default framebuffer
    glBlitFramebuffer(
                      0, 0, ofGetWidth(), ofGetHeight(), 0, 0, ofGetWidth(), ofGetHeight(), GL_DEPTH_BUFFER_BIT, GL_NEAREST
                      );
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    renderFbo.begin();
    ofEnableSmoothing();
    lightShader.begin();
    lightShader.setUniformMatrix4f("projection", projection);
    lightShader.setUniformMatrix4f("view", viewMatrix);
    for(int i=0; i<LIGHT_NUM; i++) {
        ofMatrix4x4 modelMatrix;
        modelMatrix.translate(light[i].getPosition());
//        lightShader.setUniform3f("lightColor", light[i].getColor());
        if(isGray){
            ofVec3f grayColor = ofVec3f(light[i].getGray());
            lightShader.setUniform3f("lightColor", grayColor);
        } else {
            lightShader.setUniform3f("lightColor", light[i].getColor());
        }
        lightShader.setUniformMatrix4f("model", modelMatrix);
        light[i].draw();
    }
    lightShader.end();
    renderFbo.end();
    
    ofDisableDepthTest();
    
    renderFbo.draw(0, 0, renderFbo.getWidth(), renderFbo.getHeight());
    
    lightFbo.draw(300, 10, lightFbo.getWidth() * 0.1,lightFbo.getHeight() * 0.1);

    ofDrawBitmapString("debugMode: " + ofToString(cam.getPosition()), 10, 10);
    gui.draw();
    
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
