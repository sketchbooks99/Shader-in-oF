#pragma once

#include "ofMain.h"
#include "ofxGui.h"

class ofxSimpleBlur {
public:
	ofxSimpleBlur() {}
	~ofxSimpleBlur() {}

	void allocate(int _width, int _height) {
		target.allocate(_width, _height);
		onePassFbo.allocate(_width, _height);
		result.allocate(_width, _height);
	}

	void begin() {
		target.begin();
		ofClear(0);
		ofBackground(0); 
	}

	void end() {
		target.end();

		// horizontal blur
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

	void setupShader() {
		stringstream vertSrc;
		vertSrc << "#version 150\n";
		vertSrc << "uniform mat4 modelViewProjectionMatrix;\n";
		vertSrc << "in vec4 position;\n";
		vertSrc << "in vec2 texcoord;\n";
		vertSrc << "out vec2 vTexCoord;\n";
		vertSrc << "void main() {\n";
		vertSrc << "	gl_Position = modelViewProjectionMatrix * position;\n";
		vertSrc << "	vTexCoord = texcoord;\n";
		vertSrc << "}\n";

		stringstream fragSrc;
		fragSrc << "#version 150\n";
		fragSrc << "uniform sampler2DRect tex;\n";
		fragSrc << "uniform vec2 direction;\n";
		fragSrc << "uniform float blurSize;\n";
		fragSrc << "in vec2 vTexCoord;\n";
		fragSrc << "\n";
		fragSrc << "out vec4 fragColor;\n";
		fragSrc << "\n";
		fragSrc << "void main() {\n";
		fragSrc << "	const int N = 16;\n";
		fragSrc << "	float delta = blurSize / float(N);\n";
		fragSrc << "	vec3 color = texture(tex, vTexCoord).rgb;\n";
		fragSrc << "	for(int i=0; i<N; i++) {\n";
		fragSrc << "		vec2 d = direction * float(i) * delta;\n";
		fragSrc << "		color += texture(tex, vTexCoord + d).rgb;\n";
		fragSrc << "		color += texture(tex, vTexCoord - d).rgb;\n";
		fragSrc << "	}\n";
		fragSrc << "	color /= float(N) * 2.0 + 1.0;\n";
		fragSrc << "	fragColor = vec4(color, 1.0);\n";
		fragSrc << "}\n";

		blurShader.setupShaderFromSource(GL_VERTEX_SHADER, vertSrc.str());
		blurShader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragSrc.str());
		blurShader.bindDefaults();
		blurShader.linkProgram();

		stringstream bloomFrag;
		bloomFrag << "#version 150\n";
		bloomFrag << "uniform sampler2DRect origin;\n";
		bloomFrag << "uniform sampler2DRect result;\n";
		bloomFrag << "uniform vec2 resolution;\n";
		bloomFrag << "uniform float strength;\n";
		bloomFrag << "in vec2 vTexCoord;\n";
		bloomFrag << "out vec4 fragColor;\n";
		bloomFrag << "";
		bloomFrag << "void main() {\n";
		bloomFrag << "	vec2 p = (vTexCoord * 2.0 - resolution) / min(resolution.x, resolution.y);\n";
		bloomFrag << "	vec2 rOffset = vec2(2.0, 0.0);\n";
		bloomFrag << "	vec2 bOffset = vec2(4.0, 0.0);\n";
		bloomFrag << "	vec4 ori = texture(origin, vTexCoord);\n";
		bloomFrag << "	vec4 res = texture(result, vTexCoord);\n";
		bloomFrag << "	float rOri = texture(origin, vTexCoord + rOffset).r;\n";
		bloomFrag << "	float rRes = texture(result, vTexCoord + rOffset).r;\n";
		bloomFrag << "	float bOri = texture(origin, vTexCoord + bOffset).b;\n";
		bloomFrag << "	float bRes = texture(result, vTexCoord + bOffset).b;\n";
		bloomFrag << "	vec3 destColor = vec3(rOri, ori.g, bOri) + vec3(rRes, res.g, bRes) * strength;\n";
		bloomFrag << "	fragColor = vec4(destColor, 1.0);\n";
		bloomFrag << "}\n";

		bloomShader.setupShaderFromSource(GL_VERTEX_SHADER, vertSrc.str());
		bloomShader.setupShaderFromSource(GL_FRAGMENT_SHADER, bloomFrag.str());
		bloomShader.bindDefaults();
		bloomShader.linkProgram();
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


struct Torus {
public:

	Torus() {

	}

	~Torus() {
		
	}

	Torus(int _hNum, int _vNum, int _outRadius, int _inRadius) {
		hNum = _hNum;
		vNum = _vNum;
		outRadius = _outRadius;
		inRadius = _inRadius;

		//mesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
	}

	void createTorus() {
		mesh.clear();

		for (int x = 0; x < vNum; x++) {
			for (int y = 0; y < hNum; y++) {

		// init mesh 
				float phi = ((float)x / vNum) * TWO_PI;
				float theta = ((float)y / hNum) * TWO_PI;
				float xPos = (outRadius + inRadius * cos(theta)) * cos(phi);
				float yPos = (outRadius + inRadius * cos(theta)) * sin(phi);
				float zPos = inRadius * sin(theta);

				float xCenter = ((outRadius + inRadius) / 2) * cos(phi);
				float yCenter = ((outRadius + inRadius) / 2) * sin(phi);

				ofVec3f normal = ofVec3f(xPos, yPos, zPos) - ofVec3f(xCenter, yCenter, 0.0);
				mesh.addColor(ofFloatColor(1.0, 1.0, 1.0, 1.0));
				mesh.addVertex(ofVec3f(xPos, yPos, zPos));
				mesh.addNormal(normal.getNormalized());
				mesh.addTexCoord(ofVec2f((float)x / vNum, (float)y / hNum));
			}
		}

		for (int x = 0; x < vNum; x++) {
			for (int y = 0; y < hNum; y++) {
				int a = x * hNum + y;
				int b = x * hNum + (y + 1) % hNum;
				int c = ((x + 1) % vNum) * hNum + y;
				int d = ((x + 1) % vNum) * hNum + (y + 1) % hNum;

				mesh.addIndex(a); mesh.addIndex(b); mesh.addIndex(c);
				mesh.addIndex(c); mesh.addIndex(b); mesh.addIndex(d);
			}
		}
	}

	void draw() {
		mesh.draw();
	}

	void setResolution(int _vNum, int _hNum) {
		vNum = _vNum; 
		hNum = _hNum;
		createTorus();
	}

	void setPosition(float x, float y, float z) {
		location = ofVec3f(x, y, z);
	}

	void setPosition(ofVec3f pos) {
		location = pos;
	}

	ofVec3f getPosition() {
		return location;
	}

	int* getResolution() {
		int res[2] = { vNum, hNum };
		return res;
	}

	int* getSize() {
		int size[2] = { outRadius, inRadius };
		return size;
	}
private:
	int hNum, vNum; // the number of horizontal and vertical slices
	int outRadius, inRadius; // out/in radius of torus
	ofVboMesh mesh;
	ofVec3f location;
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

		void createCylinder(int vNum, int hNum, int radius, int length);

		//ofVboMesh torus;
		ofEasyCam cam;
		float outSize, inSize; // torus size

		ofVboMesh cylinder;
		ofxSimpleBlur myBlur;

		Torus torus;
		Torus largeTorus;

		ofShader shader, grShader;

		ofxPanel gui;
		ofParameter<int> hNum;
		ofParameter<int> vNum;
		ofParameter<float> blurSize;
		ofParameter<float> strength;
		
};
