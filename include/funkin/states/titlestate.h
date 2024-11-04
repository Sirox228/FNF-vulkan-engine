#ifndef TITLE_STATE_H
#define TITLE_STATE_H

#include <SDL.h>

void titlestateCreate();
void titlestateEvent(SDL_Event* event);
void titlestateUpdate();
void titlestateStepHit(uint32_t step);
void titlestateBeatHit(uint32_t beat);
void titlestateDestroy();

#endif
