#pragma once

#include "ofMain.h"
#include "Particle.hpp"
#include "Fish.hpp"
#include "Border.hpp"
#include "Ripple.hpp"
#include "Gear.hpp"
#include "GearController.hpp"
#include "Fish2.hpp"
#include "Rain.hpp"
#include "ofxLayerMask.h"

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
    
private:
    void createItems();
    void createBorderItems();
    void createRippleItems();
    void createGearItems();
    float getVelocity(float destination, float location, float velocity);
    void setGraphicId();
    int getId();
    void changeGraphic(bool changetoNext);
    bool hasTimePassed();
    void resetTime();
    int getElapsedTime();
    float getMaskRadius();
    
    int INTERVAL_OFFSET = 20;
    int AZIMUTH_DIFF_OFFSET = 40;
    float ACCEL_OFFSET = 0.4;
    int CHANGE_DELAY_OFFSET = 100;
    
    vector<Item*> particles;
    vector<string> imageRefs;
    ofImage img;
    float stiffness;
    float damping;
    ofPoint destination;
    ofPoint dummyLocation;
    ofPoint velocity;
    ofVec3f accel, normAccel;
    float normAccelX, normAccelY;
    float latitude, longitude, speed;
    ofTrueTypeFont font;
    int graphicId;
    int interval;
    int changeDelay;
    float lastAzimuth;
    float azimuthDiff;
    int startTime;
    int timeUntilChangeGraphic;
    GearController* gearController;
    ofImage head;
    ofImage body;
    ofImage tail;
    
    Rain rain;
    ofxLayerMask masker;
};
