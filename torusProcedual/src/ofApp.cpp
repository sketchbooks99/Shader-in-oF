#include "ofApp.h"

void ofApp::createCylinder(int vNum, int hNum, int radius, int length) {
	for (int x = 0; x < vNum; x++) {
		for (int y = 0; y < hNum; y++) {
			float theta = ((float)y / hNum) * TWO_PI;
			float xPos = radius * sin(theta);
			float yPos = radius * cos(theta);
			float z = ((float)x / vNum) * length;

			cylinder.addVertex(ofVec3f(xPos, yPos, z));

			ofVec3f normal = (ofVec3f(xPos, yPos, z) - ofVec3f(0, 0, z)).getNormalized();
			cylinder.addNormal(normal);
			cylinder.addTexCoord(ofVec2f((float)x / vNum, (float)y / hNum));
		}
	}

	for (int x = 0; x < vNum - 1; x++) {
		for (int y = 0; y < hNum; y++) {
			int a = x * hNum + y;
			int b = x * hNum + (y + 1) % hNum;
			int c = (x + 1) * hNum + y;
			int d = (x + 1) * hNum + (y + 1) % hNum;

			cylinder.addIndex(a); cylinder.addIndex(b); cylinder.addIndex(c);
			cylinder.addIndex(c); cylinder.addIndex(b); cylinder.addIndex(d);
		}
	}
}

//--------------------------------------------------------------
void ofApp::setup(){
	ofBackground(0);
	ofSetVerticalSync(true);
	ofEnableAlphaBlending();
	ofEnableSmoothing();

	outSize = 200;
	inSize = 100;

	cam.setFarClip(5000.0);

	torus = Torus(5, 5, 100, 50);
	//largeTorus = Torus(20, 20, 3000, 2000);
	torus.createTorus();
	//largeTorus.createTorus();

	createCylinder(50, 10, 300, 10000);

	gui.setup();
	gui.setPosition(10, 10);
	gui.add(hNum.set("hNum", 5, 0, 100));
	gui.add(vNum.set("vNum", 5, 0, 100));

	shader.load("shader/shader.vert", "shader/shader.frag", "shader/shader.geom");
	grShader.load("shader/groundTorus.vert", "shader/groundTorus.frag", "shader/groundTorus.geom");


	myBlur.allocate(ofGetWidth(), ofGetHeight());
	myBlur.setupShader();
	myBlur.setSize(5.0);
	myBlur.setStrength(1.0);

}


//--------------------------------------------------------------
void ofApp::update(){

	float time = ofGetElapsedTimef();

	int prev_v_num = torus.getResolution()[0];
	int prev_h_num = torus.getResolution()[1];
	if (prev_v_num != vNum || prev_h_num != hNum) {
		torus.setResolution(vNum, hNum);
	}

	ofSetWindowTitle(ofToString(ofGetFrameRate()));

	myBlur.begin();

	ofEnableDepthTest();

	cam.begin();

	ofMatrix4x4 smallmodel;
	smallmodel.rotate(time * 20.0, 2.0, 0.7, 1.2);

	ofMatrix4x4 view = ofGetCurrentViewMatrix();

	ofMatrix4x4 projection = cam.getProjectionMatrix();

	ofMatrix4x4 mvpMatrix = smallmodel * view * projection;
	ofMatrix4x4 invMatrix = mvpMatrix.getInverse();

	shader.begin();
	shader.setUniformMatrix4f("mvpMatrix", mvpMatrix);
	shader.setUniformMatrix4f("invMatrix", invMatrix);

	torus.draw();

	shader.end();

	ofMatrix4x4 largeModel;
	largeModel.translate(0, 0, -time * 50.0);
	mvpMatrix = largeModel * view * projection;
	invMatrix = mvpMatrix.getInverse();

	grShader.begin();

	grShader.setUniformMatrix4f("mvpMatrix", mvpMatrix);
	grShader.setUniformMatrix4f("invMatrix", invMatrix);
	grShader.setUniform1f("time", time);

	cylinder.draw();

	grShader.end();

	cam.end();

	ofDisableDepthTest();

	myBlur.end();
}

//--------------------------------------------------------------
void ofApp::draw(){
	float time = ofGetElapsedTimef();

	myBlur.debugFbo();

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
