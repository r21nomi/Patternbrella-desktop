#include "ofApp.h"
#include <algorithm>

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(0);
    
    ofLog(OF_LOG_NOTICE, "setup -----------------------------------------");
    
    font.load("verdana.ttf", 24);
    
    setGraphicId();
    
    imageRefs.push_back("images/fish_100_0.png");
    imageRefs.push_back("images/fish_100_1.png");
    imageRefs.push_back("images/fish_100_2.png");
    imageRefs.push_back("images/fish_100_3.png");
    imageRefs.push_back("images/fish_100_4.png");
    imageRefs.push_back("images/cone_80_1.png");
    imageRefs.push_back("images/eye_57_1.png");
    img.load(imageRefs[graphicId]);
    img.setAnchorPercent(0.5, 0.5);
    
    head.load("images/fish/fish_0.png");
    body.load("images/fish/fish_1.png");
    tail.load("images/fish/fish_2.png");
    
    stiffness = 0.1;
    damping = 0.85;
    interval = changeDelay = lastAzimuth = azimuthDiff = 0;
    timeUntilChangeGraphic = 5 * 1000;
    normAccelX = normAccelY = 0;
    gearController = new GearController();
    
    resetTime();
    createItems();
}

//--------------------------------------------------------------
void ofApp::update(){
    // For Exhibition.
//     if (hasTimePassed()) {
//         changeGraphic(true);
//     }
    
    if (particles[0] != NULL && dynamic_cast<Gear*>(particles[0])) {
        // Gear.
        gearController->update(normAccelX, normAccelY);
        
    } else {
        // Other graphic.
        int count = 0;
        for (Item *particle : particles) {
            particle->update(0, 0, 0.1, 0.1);
            count++;
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    for (Item *particle : particles) {
        particle->draw();
    }
}

void ofApp::createItems() {
    switch (graphicId) {
        case 1:
            createBorderItems();
            break;
            
        case 2:
            createRippleItems();
            break;
            
        case 3:
            createGearItems();
            break;
            
        default:
            particles.clear();
            
            float width = 100;
            float margin = 0;
            
            for (int i = 0, wLen = ofGetWidth() / (width + margin); i < wLen; i++) {
                for (int j = 0, hLen = ofGetHeight() / (width + margin); j < hLen; j++) {
                    Item *particle;
                    
                    if (graphicId == 3) {
                        particle = new Particle(&img, ofPoint(i * (width + margin), j * (width + margin)), width);
                        
                    } else if (graphicId == 4) {
                        particle = new Fish2(&head, &body, &tail, ofPoint(i * (width + margin), j * (width + margin)));
                        
                    } else {
                        particle = new Fish(&img, ofPoint(i * (width + margin), j * (width + margin)), width);
                    }
                    
                    float dist = ofDist(particle->getLocation().x, particle->getLocation().y, ofGetWidth() / 2, ofGetHeight() / 2);
                    
                    if (dist < ofGetHeight() / 2) {
                        particles.push_back(particle);
                    }
                }
            }
            break;
    }
}

/**
 * Border
 */
void ofApp::createBorderItems() {
    particles.clear();
    
    int currentXLen = 0;
    int currentYLen = 0;
    // Horizontal
    for (int i = 0, wLen = ofGetWidth(); currentXLen < wLen; i++) {
        int bold = ofRandom(10, 80);
        Item *particle = new Border(ofPoint(currentXLen, 0), bold, ofGetHeight(), ofRandom(5, 8), Border::HORIZONTAL);
        particles.push_back(particle);
        currentXLen += bold;
    }
    // Vertical
    for (int j = 0, hLen = ofGetHeight(); currentYLen < hLen; j++) {
        int bold = ofRandom(10, 80);
        Item *particle = new Border(ofPoint(0, currentYLen), ofGetWidth(), bold, ofRandom(5, 8), Border::VERTICAL);
        particles.push_back(particle);
        currentYLen += bold;
    }
    random_shuffle(particles.begin(), particles.end());
}

/**
 * Ripple
 */
void ofApp::createRippleItems() {
    particles.clear();
    
    for (int i = 0, len = 50; i < len; i++) {
        Item *ripple = new Ripple(
                                  ofPoint(ofRandom(ofGetWidth()), ofRandom(ofGetHeight())),
                                  8,
                                  400,
                                  (int)ofRandom(0, 1500),
                                  ofColor(0, ofRandom(255), 255)
                                  );
        particles.push_back(ripple);
    }
}

/**
 * Gear
 */
void ofApp::createGearItems() {
    particles.clear();
    
    int diameter = 150;
    int bold = 30;
    int margin = 30;
    
    for (int x = 0, width = ofGetWidth(); x < width; x += (diameter + margin)) {
        for (int y = 0, height = ofGetHeight(); y < height; y += (diameter + margin)) {
            Gear *gear = new Gear(
                                  ofPoint(x, y),
                                  diameter,
                                  bold,
                                  4
                                  );
            Item *item = gear;
            particles.push_back(item);
        }
    }
    gearController->setItems(particles);
}

float ofApp::getVelocity(float destination, float location, float velocity) {
    float force = stiffness * (destination - location);
    return damping * (velocity + force);
}

void ofApp::setGraphicId() {
    graphicId = getId();
    ofLog(OF_LOG_NOTICE, "ID : " + ofToString(graphicId));
}

void ofApp::changeGraphic(bool changetoNext) {
    if (changetoNext) {
        graphicId++;
        if (imageRefs.size() <= graphicId) {
            graphicId = 0;
        }
    } else {
        graphicId--;
        if (graphicId < 0) {
            graphicId = imageRefs.size() - 1;
        }
    }
    img.clear();
    img.load(imageRefs[graphicId]);
    createItems();
    resetTime();
}

int ofApp::getId() {
    return 1;
}

void ofApp::resetTime() {
    startTime = ofGetElapsedTimeMillis();
}

bool ofApp::hasTimePassed() {
    return getElapsedTime() > timeUntilChangeGraphic;
}

int ofApp::getElapsedTime() {
    return ofGetElapsedTimeMillis() - startTime;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    switch (key) {
        case ' ':
            changeGraphic(true);
            break;
    }
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
