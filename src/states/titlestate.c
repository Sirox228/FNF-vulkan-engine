#include "holder.h"
#include "states/state.h"

#define gfTitle sprites[0]
#define logoTitle sprites[1]
#define pressTitle sprites[2]

void titlestateCreate() {
    state_allocate_sprites(3);

    gfTitle = animatedSpriteCreate("assets/textures/gfDanceTitle.png", "assets/atlases/gfDanceTitle.xml", 550, 50, 24);
    logoTitle = animatedSpriteCreate("assets/textures/logoBumpin.png", "assets/atlases/logoBumpin.xml", -75.0, -75.0, 24);
    pressTitle = animatedSpriteCreate("assets/textures/titleEnter.png", "assets/atlases/titleEnter.xml", 150, 580, 24);
    spritePlayAnimation("Press Enter to Begin", &pressTitle);
}

void titlestateUpdate() {

}

void titlestateDestroy() {
    vkDeviceWaitIdle(device);

    animatedSpriteDestroy(&gfTitle);
    animatedSpriteDestroy(&logoTitle);
    animatedSpriteDestroy(&pressTitle);
    state_free_sprites();
}
