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
#include "Circle.hpp"
#include "Flower.hpp"
#include "Hexagon.hpp"
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
    enum Graphic {
        BORDER,
        RAIN,
        GEAR,
        FISH2,
        CIRCLE,
        FLOWER,
        HEXAGON
    };
    void createItems();
    void createFish2Items();
    void createBorderItems();
    void createRippleItems();
    void createGearItems();
    void createCircleItems();
    void createFlowerItems();
    void createHexagon();
    
    float getVelocity(float destination, float location, float velocity);
    void setGraphicId(int id);
    int getId();
    float getAzimuth();
    bool isDebugMode();
    void reset();
    void changeGraphicIfNeeded();
    void changeGraphic(bool changetoNext);
    void updateGraphic();
    bool hasTimePassed(int time);
    void resetTime();
    int getElapsedTime();
    float getMaskRadius();
    bool hasTimePassed();
    
    /**
     * Get updated image path.
     * ex. If current image path is "data/img_1_1.png", updated image path will be "data/img_1_2.png".
     *     If updated path was not found, the path will be "data/img_1_1.png".
     */
    static inline string GetUpdatedImagePath(string oldImagePath) {
        vector<string> imagePaths = ofSplitString(oldImagePath, ".");
        vector<string> imageNumbers = ofSplitString(imagePaths[0], "_");
        int imageNumber = ofToInt(imageNumbers[imageNumbers.size() - 1]);
        string newImagePath = "";
        for (int i = 0, len = imageNumbers.size(); i < len; i++) {
            if (i < len - 1) {
                newImagePath += (imageNumbers[i] + "_");
            }
        }
        string newNumber = ofToString(imageNumber + 1);
        string extention = ("." + imagePaths[1]);
        
        ofFile file(ofToDataPath(newImagePath + newNumber + extention));
        
        if (!file.exists()) {
            newNumber = "1";
        }
        
        newImagePath += (newNumber + extention);
        
        return newImagePath;
    }
    
    Graphic currentGraphic;
    int INTERVAL_OFFSET = 30;
    int AZIMUTH_DIFF_OFFSET = 40;
    float ACCEL_OFFSET = 0.4;
    int CHANGE_DELAY_OFFSET = 100;
    
    vector<Item*> particles;
    vector<string> imageRefs;
    vector<ofImage> fishImages;
    vector<string> fishImagePaths;
    vector<ofImage> flowerImages;
    vector<string> flowerImagePaths;
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
    int flowerTypeCount;
    
    Rain rain;
    ofxLayerMask masker;
};
