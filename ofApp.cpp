#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openframeworks");

	ofBackground(239);

	this->fbo.allocate(ofGetWidth(), ofGetHeight());
	this->count = 300;
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);

	this->fbo.begin();
	ofClear(0);
	ofSetColor(0);

	ofTranslate(ofGetWidth() * 0.5, ofGetHeight() * 0.5);

	for (int k = 0; k < 8; k++) {

		auto noise_seed = glm::vec2(ofRandom(1000), ofRandom(1000));

		ofMesh mesh;
		vector<glm::vec3> right, left;

		glm::vec3 last_location;
		float last_theta;
		int len = 30;
		int head_size = 25;

		for (int i = 0; i < len; i++) {

			auto location = glm::vec3(ofMap(ofNoise(noise_seed.x, (ofGetFrameNum() + i) * 0.005), 0, 1, -325, 325), ofMap(ofNoise(noise_seed.y, (ofGetFrameNum() + i) * 0.005), 0, 1, -350, 350), 0);
			auto next = glm::vec3(ofMap(ofNoise(noise_seed.x, (ofGetFrameNum() + i + 1) * 0.005), 0, 1, -325, 325), ofMap(ofNoise(noise_seed.y, (ofGetFrameNum() + i + 1) * 0.005), 0, 1, -350, 350), 0);

			auto direction = next - location;
			auto theta = atan2(direction.y, direction.x);

			right.push_back(location + glm::vec3(ofMap(i, 0, len, 0, head_size) * cos(theta + PI * 0.5), ofMap(i, 0, len, 0, head_size) * sin(theta + PI * 0.5), 0));
			left.push_back(location + glm::vec3(ofMap(i, 0, len, 0, head_size) * cos(theta - PI * 0.5), ofMap(i, 0, len, 0, head_size) * sin(theta - PI * 0.5), 0));

			last_location = location;
			last_theta = theta;
		}

		for (int i = 0; i < right.size(); i++) {

			mesh.addVertex(left[i]);
			mesh.addVertex(right[i]);
		}

		for (int i = 0; i < mesh.getNumVertices() - 2; i += 2) {

			mesh.addIndex(i + 0); mesh.addIndex(i + 1); mesh.addIndex(i + 3);
			mesh.addIndex(i + 0); mesh.addIndex(i + 2); mesh.addIndex(i + 3);
		}

		auto tmp_head_size = ofMap(len - 2, 0, len, 0, head_size);
		mesh.addVertex(last_location);

		int index = mesh.getNumVertices();
		for (auto theta = last_theta - PI * 0.5; theta <= last_theta + PI * 0.5; theta += PI / 20) {

			mesh.addVertex(last_location + glm::vec3(tmp_head_size * cos(theta), tmp_head_size * sin(theta), 0));
		}

		for (int i = index; i < mesh.getNumVertices() - 1; i++) {

			mesh.addIndex(index); mesh.addIndex(i + 0); mesh.addIndex(i + 1);
		}

		mesh.draw();
	}

	this->fbo.end();
	this->fbo.readToPixels(this->pix);
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->circles.clear();

	while (circles.size() < this->count) {

		int x = ofRandom(ofGetWidth());
		int y = ofRandom(ofGetHeight());

		ofColor pix_color = this->pix.getColor(x, y);
		if (pix_color != ofColor(0)) {

			continue;
		}

		ofColor color;
		color.setHsb(ofRandom(255), 255, 255);
		glm::vec2 point = glm::vec2(x, y);
		float radius = ofRandom(2, 8);

		bool flag = true;
		for (int i = 0; i < circles.size(); i++) {

			if (glm::distance(point, get<1>(circles[i])) < get<2>(circles[i]) + radius) {

				flag = false;
				break;
			}
		}

		if (flag) {

			circles.push_back(make_tuple(color, point, radius));
		}
	}

	for (int circles_index = 0; circles_index < circles.size(); circles_index++) {

		ofColor color = get<0>(circles[circles_index]);
		glm::vec2 point = get<1>(circles[circles_index]);
		float radius = get<2>(circles[circles_index]);

		ofSetColor(color);
		ofDrawCircle(point, radius);
	}

	ofPopMatrix();
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}