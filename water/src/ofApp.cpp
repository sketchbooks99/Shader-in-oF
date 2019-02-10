#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(0);
    ofSetVerticalSync(true);
    sphereShader.setGeometryOutputCount(3);
    sphereShader.setGeometryOutputType(GL_TRIANGLE_STRIP);
    sphereShader.setGeometryInputType(GL_TRIANGLES);
//    sphereShader.load("shader/sphere.vert", "shader/sphere.frag", "shader/sphere.geom");
    planeShader.load("shader/plane.vert", "shader/sphere.frag");
//    ofEnableAlphaBlending();

    string imgName[] = {
        "cube/n_x.png",
        "cube/p_x.png",
        "cube/p_y.png",
        "cube/n_y.png",
        "cube/p_z.png",
        "cube/n_z.png"
    };

    sphereMesh = ofIcoSpherePrimitive(10, 2).getMesh();
    planeMesh = ofPlanePrimitive(2000, 2000, 50, 50).getMesh();
    for(int i=0; i<planeMesh.getVertices().size(); i++) {
        float z = planeMesh.getVertex(i).y;
        float y = planeMesh.getVertex(i).z;
        planeMesh.setVertex(i, ofVec3f(planeMesh.getVertex(i).x, y, z));
    }

    cubeTexture = loadCubeMap(imgName);
    createMesh();
    cam.setGlobalPosition(0, 0, 400.0);
    
    renderFbo.allocate(ofGetWidth(), ofGetHeight());
}

//--------------------------------------------------------------
void ofApp::update(){
    
//    renderFbo.begin();
//    ofClear(0, 0, 0);
//    ofEnableDepthTest();
//    cam.begin();
//    // model Matrix
//    modelMatrix.translate(0, 0, 0);
//
//    // view Matrix
//    viewMatrix = ofGetCurrentViewMatrix();
//
//    // projection Matrix
//    projectionMatrix = cam.getProjectionMatrix();
//
//    // model view projection Matrix
//    mvpMatrix = modelMatrix * viewMatrix * projectionMatrix;
//
//    ofMatrix4x4 invMatrix = mvpMatrix.getInverse();
//    ofMatrix4x4 vpMatrix = viewMatrix * projectionMatrix;
//    sphereShader.begin();
//
//    sphereShader.setUniform3f("eyePosition", cam.getPosition());
//    sphereShader.setUniform1i("cubeTexture", 0);
//    sphereShader.setUniformMatrix4f("modelMatrix", modelMatrix);
//    sphereShader.setUniformMatrix4f("modelViewProjectionMatrix", mvpMatrix);
//    sphereShader.setUniformMatrix4f("vpMatrix", vpMatrix);
//    sphereShader.setUniformMatrix4f("invMatrix", invMatrix);
//    sphereShader.setUniform1i("mirror", true);
//    sphereShader.setUniform1f("time", ofGetElapsedTimef());
//
//    sphereMesh.draw();
//
//    sphereShader.end();
//    cam.end();
//    ofDisableDepthTest();
//    renderFbo.end();

}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofEnableDepthTest();

    cam.begin();
    
        // model Matrix
    ofMatrix4x4 modelMatrix;
    modelMatrix.translate(0, 0, 0);
    modelMatrix.rotate(ofGetElapsedTimef()*10.0,0.0,2.0,0.0);
    
    // view Matrix
    ofMatrix4x4 viewMatrix;
    viewMatrix = ofGetCurrentViewMatrix();
    
    // projection Matrix
    ofMatrix4x4 projectionMatrix;
    projectionMatrix = cam.getProjectionMatrix();
    
    // model view projection Matrix
    ofMatrix4x4 mvpMatrix;
    mvpMatrix = modelMatrix * viewMatrix * projectionMatrix;
    
    ofMatrix4x4 invMatrix = mvpMatrix.getInverse();
    ofMatrix4x4 vpMatrix = viewMatrix * projectionMatrix;
    

    // 一度鏡の中の世界を書き込む
//    glClearStencil(0);
//    glEnable(GL_STENCIL_TEST);
//
//    glStencilFunc(GL_ALWAYS, 1, 0);
//    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

//    sphereShader.begin();
//
//    sphereShader.setUniform3f("eyePosition", cam.getPosition());
//    sphereShader.setUniform1i("cubeTexture", 0);
//    sphereShader.setUniformMatrix4f("modelMatrix", modelMatrix);
//    sphereShader.setUniformMatrix4f("modelViewProjectionMatrix", mvpMatrix);
//    sphereShader.setUniform1i("mirror", false);
//    sphereShader.setUniformMatrix4f("vpMatrix", vpMatrix);
//    sphereShader.setUniformMatrix4f("invMatrix", invMatrix);
//    sphereShader.setUniform1f("time", ofGetElapsedTimef());
//
//    sphereMesh.draw();
//    sphereShader.end();
    
//    renderFbo.draw(0, 0);

//    glEnable(GL_STENCIL_TEST);
//    glStencilFunc(GL_ALWAYS, 1, 0);
//    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

    planeShader.begin();
    planeShader.setUniform3f("eyePosition", cam.getPosition());
    planeShader.setUniform1i("cubeTexture", 0);
    planeShader.setUniformMatrix4f("modelViewProjectionMatrix", mvpMatrix);
    planeShader.setUniformMatrix4f("invMatrix", invMatrix);
    planeShader.setUniform1f("time", ofGetElapsedTimef());
    ofPushMatrix();
    ofTranslate(0, -200.0, 0);

    planeMesh.draw();
    ofPopMatrix();
    planeShader.end();

//    glDisable(GL_STENCIL_TEST);

    for(int i=0; i<6; i++) {
        image[i].bind();
        boxFaces[i].draw(OF_MESH_FILL);
        image[i].unbind();
    }


    cam.end();

    ofDisableDepthTest();
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

//--------------------------------------------------------------
unsigned int ofApp::loadCubeMap(string name[]) {
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    ofImage image[6];
    for(int i=0; i<6; i++) {
        image[i].load(name[i]);
        this->image[i] = image[i];
    }

    unsigned char *pixels;
    for(int i=0; i<6; i++){
        pixels = image[i].getPixels().getData();
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i,0,GL_RGBA,image[i].getWidth(),image[i].getHeight(),0,GL_RGBA,GL_UNSIGNED_BYTE,pixels);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}

//--------------------------------------------------------------
void ofApp::createMesh() {
    // right Face
    int w = this->width*10;
    boxFaces[0].addVertex(ofVec3f(w,-w,w));
    boxFaces[0].addTexCoord(ofVec2f(0,width));
    boxFaces[0].addVertex(ofVec3f(w,-w,-w));
    boxFaces[0].addTexCoord(ofVec2f(width,width));
    boxFaces[0].addVertex(ofVec3f(w,w,-w));
    boxFaces[0].addTexCoord(ofVec2f(width,0));
    boxFaces[0].addVertex(ofVec3f(w,w,w));
    boxFaces[0].addTexCoord(ofVec2f(0,0));

    // left Face
    boxFaces[1].addVertex(ofVec3f(-w,-w,w));
    boxFaces[1].addTexCoord(ofVec2f(width,width));
    boxFaces[1].addVertex(ofVec3f(-w,w,w));
    boxFaces[1].addTexCoord(ofVec2f(width,0));
    boxFaces[1].addVertex(ofVec3f(-w,w,-w));
    boxFaces[1].addTexCoord(ofVec2f(0,0));
    boxFaces[1].addVertex(ofVec3f(-w,-w,-w));
    boxFaces[1].addTexCoord(ofVec2f(0,width));

    // top Face
    boxFaces[2].addVertex(ofVec3f(w,w,w));
    boxFaces[2].addTexCoord(ofVec2f(width,width));
    boxFaces[2].addVertex(ofVec3f(w,w,-w));
    boxFaces[2].addTexCoord(ofVec2f(width,0));
    boxFaces[2].addVertex(ofVec3f(-w,w,-w));
    boxFaces[2].addTexCoord(ofVec2f(0,0));
    boxFaces[2].addVertex(ofVec3f(-w,w,w));
    boxFaces[2].addTexCoord(ofVec2f(0,width));

    // bottom Face
    boxFaces[3].addVertex(ofVec3f(-w,-w,w));
    boxFaces[3].addTexCoord(ofVec2f(0,0));
    boxFaces[3].addVertex(ofVec3f(-w,-w,-w));
    boxFaces[3].addTexCoord(ofVec2f(0,width));
    boxFaces[3].addVertex(ofVec3f(w,-w,-w));
    boxFaces[3].addTexCoord(ofVec2f(width,width));
    boxFaces[3].addVertex(ofVec3f(w,-w,w));
    boxFaces[3].addTexCoord(ofVec2f(width,0));

    // front Face
    boxFaces[4].addVertex(ofVec3f(w,w,w));
    boxFaces[4].addTexCoord(ofVec2f(width,0));
    boxFaces[4].addVertex(ofVec3f(-w,w,w));
    boxFaces[4].addTexCoord(ofVec2f(0,0));
    boxFaces[4].addVertex(ofVec3f(-w,-w,w));
    boxFaces[4].addTexCoord(ofVec2f(0,width));
    boxFaces[4].addVertex(ofVec3f(w,-w,w));
    boxFaces[4].addTexCoord(ofVec2f(width,width));

    // backFace
    boxFaces[5].addVertex(ofVec3f(-w,-w,-w));
    boxFaces[5].addTexCoord(ofVec2f(width,width));
    boxFaces[5].addVertex(ofVec3f(-w,w,-w));
    boxFaces[5].addTexCoord(ofVec2f(width,0));
    boxFaces[5].addVertex(ofVec3f(w,w,-w));
    boxFaces[5].addTexCoord(ofVec2f(0,0));
    boxFaces[5].addVertex(ofVec3f(w,-w,-w));
    boxFaces[5].addTexCoord(ofVec2f(0,width));

    for(int i=0; i<6; i++) {
        boxFaces[i].setMode(OF_PRIMITIVE_TRIANGLE_FAN);
    }
}
