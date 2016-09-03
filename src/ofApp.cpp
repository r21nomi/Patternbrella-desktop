#include "ofApp.h"
#include <algorithm>

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(0);
    
    masker.setup(ofGetWidth(), ofGetHeight());
    masker.newLayer();
    
    font.load("verdana.ttf", 24);
    
    setGraphicId(getId());
    
    imageRefs.push_back("images/fish_100_1.png");
    imageRefs.push_back("images/fish_100_2.png");
    imageRefs.push_back("images/fish_100_3.png");
    imageRefs.push_back("images/cone_80_1.png");
    imageRefs.push_back("images/eye_57_1.png");
    imageRefs.push_back("images/fish_100_4.png");
    imageRefs.push_back("images/fish_100_5.png");
    imageRefs.push_back("images/fish_100_5.png");
    img.load(imageRefs[graphicId]);
    img.setAnchorPercent(0.5, 0.5);
    
    // Fish
    fishImagePaths.push_back("images/fish/fish_head_1.png");
    fishImagePaths.push_back("images/fish/fish_body_1.png");
    fishImagePaths.push_back("images/fish/fish_tail_1.png");
    for (string path : fishImagePaths) {
        ofImage image;
        image.load(path);
        fishImages.push_back(image);
    }
    
    // Flower
    flowerTypeCount = 10;
    for (int i = 0; i < flowerTypeCount; i++) {
        flowerImagePaths.push_back("images/flower/flower_" + ofToString(i + 1) + ".png");
    }
    for (string path : flowerImagePaths) {
        ofImage image;
        image.load(path);
        flowerImages.push_back(image);
    }
    
    stiffness = 0.1;
    damping = 0.85;
    interval = changeDelay = lastAzimuth = azimuthDiff = 0;
    timeUntilChangeGraphic = 5 * 1000;
    normAccelX = normAccelY = 0;
    gearController = new GearController();
    
    resetTime();
    createItems();
    
    rain.start();
    
}

//--------------------------------------------------------------
void ofApp::update(){
    // For Exhibition.
    if (hasTimePassed()) {
        changeGraphic(true);
    }
    
    rain.updateState();
    
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
    ofBackground(ofColor::black);
    
    // Contents
    masker.beginLayer();
    {
        ofClear(0, 0, 0, 255);
        for (Item *particle : particles) {
            particle->draw();
        }
    }
    masker.endLayer();
    
    // Mask
    masker.beginMask();
    {
        ofClear(0, 0, 0, 255);
        ofSetColor(ofColor::white);
        ofDrawCircle(ofGetWidth() / 2, ofGetHeight() / 2, getMaskRadius());
    }
    masker.endMask();
    
    masker.draw();
}

void ofApp::createItems() {
    switch (currentGraphic) {
        case BORDER:
            createBorderItems();
            break;
            
        case RAIN:
            createRippleItems();
            break;
            
        case GEAR:
            createGearItems();
            break;
            
        case CIRCLE:
            createCircleItems();
            break;
            
        case FISH2:
            createFish2Items();
            break;
            
        case FLOWER:
            createFlowerItems();
            break;
            
        case HEXAGON:
            createHexagon();
            break;
            
        default:
            particles.clear();
            
            float width = 100;
            float margin = 0;
            
            for (int i = 0, wLen = ofGetWidth() / (width + margin); i < wLen; i++) {
                for (int j = 0, hLen = ofGetHeight() / (width + margin); j < hLen; j++) {
                    Item *particle = new Fish(&img, ofPoint(i * (width + margin), j * (width + margin)), width);
                    
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

void ofApp::createFish2Items() {
    particles.clear();
    
    for (int i = 0, len = 50; i < len; i++) {
        Item *particle = new Fish2(
                                   &fishImages[0],
                                   &fishImages[1],
                                   &fishImages[2],
                                   ofPoint(ofRandom(ofGetWidth()), ofRandom(ofGetHeight()))
                                   );
        particles.push_back(particle);
    }
}

/**
 * Flower
 */
void ofApp::createFlowerItems() {
    particles.clear();
    
    int maxCount = 5000;
    int currentCount = 1;
    float x[maxCount];
    float y[maxCount];
    float r[maxCount];
    int closestIndex[maxCount];
    
    float minRadius = 5;
    float maxRadius = 150;
    
    x[0] = ofGetWidth() / 2;
    y[0] = ofGetHeight() / 2;
    r[0] = maxRadius / 2;
    closestIndex[0] = 0;
    
    for (int k = 0; k < 2000; k++) {
        float newX = ofRandom(0 + maxRadius, ofGetWidth() - maxRadius);
        float newY = ofRandom(0 + maxRadius, ofGetHeight() - maxRadius);
        float newR = minRadius;
        
        bool intersection = false;
        
        for (int j = 0; j < currentCount; j++) {
            float d = ofDist(newX, newY, x[j], y[j]);
            if (d < (newR + r[j])) {
                intersection = true;
                break;
            }
        }
        
        if (intersection == false) {
            float newRadius = ofGetWidth();
            for (int i = 0; i < currentCount; i++) {
                float d = ofDist(newX, newY, x[i], y[i]);
                if (newRadius > d - r[i]) {
                    newRadius = d - r[i];
                    closestIndex[currentCount] = i;
                }
            }
            
            if (newRadius > maxRadius) {
                newRadius = maxRadius;
            }
            
            x[currentCount] = newX;
            y[currentCount] = newY;
            r[currentCount] = newRadius;
            currentCount++;
        }
    }
    
    for (int i = 0 ; i < currentCount; i++) {
        Item *item = new Flower(
                                &flowerImages[i % flowerTypeCount],
                                ofPoint((int)x[i], (int)y[i]),
                                r[i] * 2,
                                r[i] * 2
                                );
        particles.push_back(item);
    }
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

/**
 * Circle
 */
void ofApp::createCircleItems() {
    particles.clear();
    
    for (int i = 50, len = 50; i > 0; i -= 4) {
        Circle *circle = new Circle(
                                    ofPoint(ofGetWidth() / 2, ofGetHeight() / 2),
                                    ofGetWidth() / len * i,
                                    ofGetWidth() / len * i,
                                    6,
                                    ofGetWidth(),
                                    ofColor(ofRandom(255), ofRandom(255), ofRandom(255))
                                    );
        Item *item = circle;
        particles.push_back(item);
    }
}

/**
 * Hexagon
 */
void ofApp::createHexagon() {
    particles.clear();
    
    int diameter = 300;
    int row = 0;
    for (int x = 0, width = ofGetWidth() + diameter * 2; x < width; x += diameter) {
        int line = 0;
        row++;
        
        for (int y = 0, height = ofGetHeight() + diameter * 2; y < height; y += diameter) {
            vector<ofColor> colors;
            colors.push_back(ofColor(230, 40, 100));
            colors.push_back(ofColor(70, 190, 210));
            
            line++;
            // 対角線の距離
            float diagonalDist = sqrt(3) * diameter / 4 * 2;
            int verticalOffset = line % 2 == 0 ? 0 : diagonalDist / 2;
            Hexagon *item = new Hexagon(
                                        ofPoint(
                                                x - (diameter - diagonalDist) * row + verticalOffset,
                                                y - diagonalDist / (2 * sqrt(3)) * line
                                                ),
                                        diameter / 2,
                                        colors,
                                        (int)ofRandom(2) % 2 == 0 ? 0 : 1,
                                        ofRandom(1, 3)
                                        );
            particles.push_back(item);
        }
    }
}

float ofApp::getVelocity(float destination, float location, float velocity) {
    float force = stiffness * (destination - location);
    return damping * (velocity + force);
}

void ofApp::setGraphicId(int id) {
    graphicId = id;
    
    switch (graphicId) {
        case 1:
            currentGraphic = BORDER;
            break;
            
        case 2:
            currentGraphic = RAIN;
            break;
            
        case 3:
            currentGraphic = GEAR;
            break;
            
        case 4:
            currentGraphic = FISH2;
            break;
            
        case 5:
            currentGraphic = CIRCLE;
            break;
            
        case 6:
            currentGraphic = FLOWER;
            break;
            
        case 7:
            currentGraphic = HEXAGON;
            break;
            
        default:
            currentGraphic = FISH;
            break;
    }
}

void ofApp::changeGraphic(bool changetoNext) {
    int totalGraphicCount = 8;
    
    if (changetoNext) {
        setGraphicId(graphicId + 1);
        if (graphicId > totalGraphicCount) {
            setGraphicId(1);
        }
    } else {
        setGraphicId(graphicId - 1);
        if (graphicId < 0) {
            setGraphicId(totalGraphicCount);
        }
    }
//    img.clear();
//    img.load(imageRefs[graphicId]);
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

float ofApp::getMaskRadius() {
    return ofGetHeight() / 2;
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
