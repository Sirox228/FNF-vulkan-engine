#include "holder.h"
#include "funkin/timing/conductor.h"
#include <math.h>

float oldbeat;
float oldstep;

void updateConductor() {
    songPosition = getSongPosition();
    if (songPosition > 0) {
        currentBeat = getCurrentBeat();
        currentStep = getCurrentStep();
        if (oldbeat != currentBeat){
            oldbeat = currentBeat;
            printf("BeatHit\n");
        }
        if (oldstep != currentStep){
            oldstep = currentStep;
            //printf("StepHit\n");
        }
        if (conductorTime / 1000 >= 10)
        conductorTime += timeDelta;
    }
}

void resetConductor(float newBPM) {
    bpm = getBPM(newBPM);
    songPosition = 0;
    currentBeat = 0;
    currentStep = 0;
    beatLengthMs = getBeatLength();
    stepLengthMs = getStepLength();

    conductorTime = 0;
}

float getBPM(float newBPM) {
    return (newBPM > 0) ? newBPM : DEFAULT_BPM;
}

float getSongPosition() {
    return 1; //Need to figure out how to get current music time
}

float getCurrentBeat() {
    return floor(conductorTime/beatLengthMs);
}

float getCurrentStep() {
    return floor(conductorTime/stepLengthMs);
}

float getBeatLength() {
    return (bpm / SECS_PER_MIN) * MS_PER_SEC;
}

float getStepLength(){
    return getBeatLength() / STEPS_PER_BEAT;
}