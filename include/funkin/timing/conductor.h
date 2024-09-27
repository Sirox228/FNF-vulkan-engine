#ifndef CONDUCTOR_H
#define CONDUCTOR_H

void updateConductor();
void resetConductor(float newBPM);

float getBPM(float newBPM);
float getSongPosition();
float getCurrentBeat();
float getCurrentStep();
float getBeatLength();
float getStepLength();

#endif