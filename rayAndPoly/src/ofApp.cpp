#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofBackground(0);
	ofSetVerticalSync(true);

	rayShader.load("shaders/passThru.vert", "shaders/raymarch.frag");
	gShader.load("shaders/gBuffer.vert", "shaders/gBuffer.frag");

	raymarchFbo.allocate(ofGetWidth(), ofGetHeight());
	renderFbo.allocate(ofGetWidth(), ofGetHeight());

	// camera settings
	cam.setNearClip(0.1);
	cam.setFarClip(500);
	cam.setFov(70);
	cam.setDistance(5.0);

	renderFbo.begin();
	ofClear(0);
	renderFbo.end();

	ofFbo::Settings gSettings;
	gSettings.textureTarget = GL_TEXTURE_2D;
	gSettings.width = ofGetWidth();
	gSettings.height = ofGetHeight();
	gSettings.internalformat = GL_RGB32F;
	gSettings.useDepth = true;
	gSettings.useStencil = true;
	gSettings.depthStencilAsTexture = true;
	gSettings.depthStencilInternalFormat = GL_DEPTH_COMPONENT;
	gSettings.maxFilter = GL_NEAREST;
	gSettings.minFilter = GL_NEAREST;
	gSettings.wrapModeVertical = GL_CLAMP_TO_EDGE;
	gSettings.wrapModeHorizontal = GL_CLAMP_TO_EDGE;
	gFbo.allocate(gSettings);

	gFbo.createAndAttachTexture(GL_RGB32F, 1); // position
	gFbo.createAndAttachTexture(GL_RGB32F, 2); // normal
	gFbo.createAndAttachTexture(GL_RGB32F, 3); // color
	gFbo.checkStatus();

	// box 
	box.set(4);

	// quad
	quad.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
	quad.addVertex(ofVec3f(1.0, 1.0, 0.0)); // top-right
	quad.addTexCoord(ofVec2f(ofGetWidth(), ofGetHeight()));
	quad.addVertex(ofVec3f(1.0, -1.0, 0.0)); //bottom-right
	quad.addTexCoord(ofVec2f(ofGetWidth(), 0.0f));
	quad.addVertex(ofVec3f(-1.0, -1.0, 0.0)); //bottom-left
	quad.addTexCoord(ofVec2f(0.0f, 0.0f));
	quad.addVertex(ofVec3f(-1.0, 1.0, 0.0)); //top-left
	quad.addTexCoord(ofVec2f(0.0f, ofGetHeight()));

	// gui settings
	gui.setup();
	gui.setPosition(10, 10);
	gui.add(debugMode.set("Debug Mode", 0, 0, 2));
}

//--------------------------------------------------------------
void ofApp::update(){
	renderFbo.begin();

	rayShader.begin();
	rayShader.setUniform2f("resolution", ofGetWidth(), ofGetHeight());
	rayShader.setUniform1f("time", ofGetElapsedTimef());

	raymarchFbo.draw(0, 0);

	rayShader.end();

	renderFbo.end();

	ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void ofApp::draw() {
	float time = ofGetElapsedTimef();
	ofBackground(0);
	glEnable(GL_DEPTH_TEST);

	// render geometry to G-buffer
	gFbo.begin();
	gFbo.activateAllDrawBuffers();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	cam.begin();
	ofMatrix4x4 projection = cam.getProjectionMatrix();
	ofMatrix4x4 model;
	model.rotate(time * 10.0, 1.0, 0.7, 0.4);
	ofMatrix4x4 view = ofGetCurrentViewMatrix();
	ofMatrix4x4 mvpMatrix = model * view * projection;
	ofMatrix4x4 invMatrix = mvpMatrix.getInverse();

	gShader.begin();
	gShader.setUniformMatrix4f("mvpMatrix", mvpMatrix);
	gShader.setUniformMatrix4f("invMatrix", invMatrix);
	gShader.setUniformMatrix4f("model", model);
	gShader.setUniformMatrix4f("view", view);
	gShader.setUniformMatrix4f("projection", projection);
	box.draw();
	gShader.end();

	cam.end();
	gFbo.end();

	// render raymarching
	gFbo.begin();
	gFbo.activateAllDrawBuffers();
	cam.begin();
	
	rayShader.begin();
	rayShader.setUniform1f("time", ofGetElapsedTimef());
	rayShader.setUniform2f("resolution", ofGetWidth(), ofGetHeight());
	rayShader.setUniform3f("camPos", cam.getGlobalPosition());
	rayShader.setUniform3f("camUp", cam.getUpDir());
	rayShader.setUniform1f("fov", cam.getFov());
	rayShader.setUniform1f("farClip", cam.getFarClip());
	rayShader.setUniform1f("nearClip", cam.getNearClip());
	rayShader.setUniformMatrix4f("view", view);
	rayShader.setUniformMatrix4f("projection", projection);
	
	quad.draw();

	rayShader.end();
	cam.end();
	gFbo.end();


	glDisable(GL_DEPTH_TEST);

	gFbo.getTexture(debugMode).draw(0, 0);

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
