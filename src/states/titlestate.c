#include "holder.h"
#include "states/state.h"

#define gfTitle sprites[0]
#define pressTitle sprites[1]
#define logoTitle sprites[2]

void titlestateCreate() {
    state_allocate_sprites(3);

    gfTitle = animatedSpriteCreate("assets/textures/gfDanceTitle.png", "assets/atlases/gfDanceTitle.xml", 500, 360, 24);
    gfTitle.scaleX = 1.0f;
    gfTitle.scaleY = 1.0f;

    globalSpriteCount = 1;
}

void titlestateUpdate() {

}

void titlestateDestroy() {
    vkDeviceWaitIdle(device);

    animatedSpriteDestroy(&gfTitle);
    state_free_sprites();
}
