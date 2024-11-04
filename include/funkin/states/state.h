#ifndef STATE_H
#define STATE_H

#define state_allocate_sprites(num) \
    sprites = (sprite*)malloc(num * sizeof(sprite)); \
    globalSpriteCount = num;

#define state_allocate_sounds(num) \
    sounds = (ma_sound*)malloc(num * sizeof(ma_sound));

#define state_free_sprites() \
    free(sprites); \
    globalSpriteCount = 0;

#define state_free_sounds() \
    free(sounds);

#define state_switch_state(create, event, update, stepHit, beatHit, destroy) \
    stateDestroy(); \
    stateCreate = create; \
    stateEvent = event; \
    stateUpdate = update; \
    stateStepHit = stepHit; \
    stateBeatHit = beatHit; \
    stateDestroy = destroy; \
    stateCreate();

#endif
