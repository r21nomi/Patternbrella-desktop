//
//  Rain.hpp
//  patternbrella
//
//  Created by 新家 亮太 on 2016/08/28.
//
//

#ifndef Rain_hpp
#define Rain_hpp

#include "ofMain.h"

class Rain {
public:
    enum RainState {
        LIGHT,
        HEAVY
    };
    
    Rain();
    void start();
    void stop();
    void updateState();
    bool isHeavyRain();
    
    RainState currentRainState;
    ofSoundPlayer soundPlayer;
    vector<string> sounds;
    int startTime;
    
private:
    void changeState(RainState state);
    bool shouldStartHeavyRain();
    bool shouldFinishHeavyRain();
    
    int LIGHT_RAIN_DURATION = 120 * 1000;
    int HEAVY_RAIN_DURATION = 20 * 1000;
};

#endif /* Rain_hpp */