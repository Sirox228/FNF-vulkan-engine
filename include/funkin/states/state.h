#ifndef STATE_H
#define STATE_H

#define state_allocate_sprites(num) \
    sprites = (sprite*)malloc(num * sizeof(sprite)); \
    globalSpriteCount = num;

#define state_free_sprites() \
    free(sprites); \
    globalSpriteCount = 0;

#define state_switch_state(create, event, update, destroy) \
    stateDestroy(); \
    stateCreate = create; \
    stateEvent = event; \
    stateUpdate = update; \
    stateDestroy = destroy; \
    stateCreate();

#endif
