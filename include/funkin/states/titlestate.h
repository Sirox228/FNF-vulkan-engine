#ifndef TITLE_STATE_H
#define TITLE_STATE_H

#include <SDL2/SDL.h>

void titlestateCreate();
void titlestateEvent(SDL_Event* event);
void titlestateUpdate();
void titlestateDestroy();

#endif
