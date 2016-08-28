//
//  Rain.cpp
//  patternbrella
//
//  Created by 新家 亮太 on 2016/08/28.
//
//

#include "Rain.hpp"

Rain::Rain() {
    currentRainState = LIGHT;
    sounds.push_back("sound/light_rain.wav");
    sounds.push_back("sound/heavy_rain.wav");
    soundPlayer.setLoop(true);
    soundPlayer.load(sounds[0]);
}

void Rain::start() {
    soundPlayer.play();
    startTime = ofGetElapsedTimeMillis();
}

void Rain::stop() {
    soundPlayer.stop();
}

void Rain::updateState() {
    if (shouldStartHeavyRain()) {
        changeState(HEAVY);
        start();
        
    } else if (shouldFinishHeavyRain()) {
        changeState(LIGHT);
        start();
    }
}

bool Rain::isHeavyRain() {
    return currentRainState == HEAVY;
}

bool Rain::shouldStartHeavyRain() {
    int elapsedTime = ofGetElapsedTimeMillis() - startTime;
    return currentRainState != HEAVY && elapsedTime > LIGHT_RAIN_DURATION;
}

bool Rain::shouldFinishHeavyRain() {
    int elapsedTime = ofGetElapsedTimeMillis() - startTime;
    return currentRainState == HEAVY && elapsedTime > HEAVY_RAIN_DURATION;
}

void Rain::changeState(RainState state) {
    switch (state) {
        case LIGHT:
            soundPlayer.load(sounds[0]);
            currentRainState = LIGHT;
            break;
            
        case HEAVY:
            soundPlayer.load(sounds[1]);
            currentRainState = HEAVY;
            break;
    }
}