#include "holder.h"

uint32_t getStep() {
    return SDL_GetTicks() / stepCrochet;
}

uint32_t getStepInt() {
return floor(SDL_GetTicks() / stepCrochet);
}

uint32_t getBeat() {
    return SDL_GetTicks() / beatCrochet;
}

uint32_t getBeatInt() {
    return floor(SDL_GetTicks() / beatCrochet);
}

void setBpm(float newBpm) {
    bpm = newBpm;
    beatCrochet = (60.0f / bpm) * 1000.0f;
    stepCrochet = beatCrochet / 4;
}
