#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);
    
    renderFbo.allocate(ofGetWidth(), ofGetHeight());
    postFbo.allocate(ofGetWidth(), ofGetHeight());
    onePassFbo.allocate(ofGetWidth(), ofGetHeight());
    twoPassFbo.allocate(ofGetWidth(), ofGetHeight());
    
    vboMesh = ofSpherePrimitive(200, 100).getMesh();
    for(int i=0; i<vboMesh.getVertices().size(); i++) {
        vboMesh.addColor(ofFloatColor(1.0, 1.0, 1.0, 1.0));
    }
    vboMesh.setMode(OF_PRIMITIVE_TRIANGLES);
    
    boxelShader.load("shader/boxel.vert", "shader/boxel.frag", "shader/boxel.geom");
    boxelShader.setGeometryInputType(GL_POINTS);
    boxelShader.setGeometryOutputCount(36);
    boxelShader.setGeometryOutputType(GL_TRIANGLE_STRIP);
    
    postShader.load("shader/passthru.vert", "shader/post.frag");
    blur.load("shader/passthru.vert", "shader/blur.frag");
    
    gui.setup();
    gui.add(boxelScale.set("scale", 5.0, 1.0, 20.0));
    gui.add(threshold.set("Threshold", 1.0, 0.1, 2.0));
    gui.add(blurSize.set("blurSize", 2.0, 1.0, 5.0));
    gui.add(wireFrame.setup("WireFrame", false));
    gui.setPosition(20, 20);
}

//--------------------------------------------------------------
void ofApp::update(){
    renderFbo.begin();
    
    ofBackground(0);
    
    cam.begin();
    
    ofEnableDepthTest();
    
    ofMatrix4x4 modelMatrix;
    modelMatrix.translate(0, 0, 0);
    modelMatrix.rotate(ofGetElapsedTimef() * 20.0, 1.0, 1.0, 0.0);
    
    ofMatrix4x4 viewMatrix;
    viewMatrix = ofGetCurrentViewMatrix();
    
    ofMatrix4x4 projectionMatrix;
    projectionMatrix = cam.getProjectionMatrix();
    
    ofMatrix4x4 mvpMatrix;
    mvpMatrix = modelMatrix * viewMatrix * projectionMatrix;
    
    ofMatrix4x4 invMatrix = mvpMatrix.getInverse();
    
    boxelShader.begin();
    boxelShader.setUniform1f("scale", boxelScale);
    boxelShader.setUniformMatrix4f("modelViewProjectionMatrix", mvpMatrix);
    boxelShader.setUniformMatrix4f("invMatrix", invMatrix);
    boxelShader.setUniform1f("time", ofGetElapsedTimef());
    
    if(wireFrame){
        vboMesh.draw(OF_MESH_WIREFRAME);
    } else {
        vboMesh.draw();
    }
    boxelShader.end();
    
    ofDisableDepthTest();
    
    cam.end();
    
    renderFbo.end();

    
    // blur
    onePassFbo.begin();
    ofBackground(0);
    blur.begin();
    blur.setUniformTexture("tex", renderFbo.getTexture(), 0);
    blur.setUniform1f("size", blurSize);
    blur.setUniform1f("threshold", threshold);
    blur.setUniform2f("direction", 5.0, 0.0);
    
    renderFbo.draw(0, 0);
    
    blur.end();
    
    onePassFbo.end();
    
    // vertical
    twoPassFbo.begin();
    ofBackground(0);
    blur.begin();
    blur.setUniformTexture("tex", onePassFbo.getTexture(), 0);
    blur.setUniform1f("size", blurSize);
    blur.setUniform1f("threshold", threshold);
    blur.setUniform2f("direction", 0.0, 5.0);
    
    onePassFbo.draw(0, 0);
    
    blur.end();
    
    twoPassFbo.end();
}

//--------------------------------------------------------------
void ofApp::draw(){
    postShader.begin();
    postShader.setUniformTexture("blurTex", twoPassFbo.getTexture(), 1);
    postShader.setUniformTexture("renderTex", renderFbo.getTexture(), 0);
    renderFbo.draw(0, 0);
    postShader.end();
    
//    twoPassFbo.draw(0, 0);
    
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
