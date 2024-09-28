#include "holder.h"
#include "states/state.h"
#include "vk/pipeline.h"

void waveTitleRender(VkCommandBuffer cmdBuf, sprite* pSprite) {
   vkCmdBindPipeline(cmdBuf, VK_PIPELINE_BIND_POINT_GRAPHICS, pSprite->customPipeline);
   VkDeviceSize quadVertexBufferOffset = 0;
   vkCmdBindVertexBuffers(cmdBuf, 0, 1, &quadVertexBuffer, &quadVertexBufferOffset);
   vkCmdBindIndexBuffer(cmdBuf, quadIndexBuffer, 0, VK_INDEX_TYPE_UINT32);
   uint32_t ticks = SDL_GetTicks();
   vkCmdPushConstants(cmdBuf, pSprite->customPipelineLayout, VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(uint32_t), &ticks);

   vkCmdDrawIndexed(cmdBuf, QUAD_IDX_NUM, 1, 0, 0, 0);
}

#define gfTitle sprites[0]
#define logoTitle sprites[1]
#define waveTitle sprites[2]
#define pressTitle sprites[3]

void titlestateCreate() {
    state_allocate_sprites(4);

    gfTitle = animatedSpriteCreate("assets/textures/gfDanceTitle.png", "assets/atlases/gfDanceTitle.xml", 525, 50, 24);
    logoTitle = animatedSpriteCreate("assets/textures/logoBumpin.png", "assets/atlases/logoBumpin.xml", -150.0, -110.0, 24);
    pressTitle = animatedSpriteCreate("assets/textures/titleEnter.png", "assets/atlases/titleEnter.xml", 115, 580, 24);
    spritePlayAnimation("Press Enter to Begin", &pressTitle);

    waveTitle.isCustomRenderFunction = 1;
    waveTitle.customRenderFunction = waveTitleRender;
    VkPushConstantRange pushConstantRange = {};
    pushConstantRange.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
    pushConstantRange.offset = 0;
    pushConstantRange.size = sizeof(uint32_t);
    createPipelineLayout(NULL, 0, &pushConstantRange, 1, &waveTitle.customPipelineLayout);
    createGraphicsPipelineDefault(waveTitle.customPipelineLayout, baseRenderPass, 0, "assets/shaders/waveTitle/waveTitle.vert.spv", "assets/shaders/waveTitle/waveTitle.frag.spv", NULL, NULL, &defaultVertexInputStateInfo, NULL, &waveTitle.customPipeline);
}

void titlestateEvent(SDL_Event* event) {

}

void titlestateUpdate() {

}

void titlestateDestroy() {
    vkDeviceWaitIdle(device);

    vkDestroyPipeline(device, waveTitle.customPipeline, NULL);
    vkDestroyPipelineLayout(device, waveTitle.customPipelineLayout, NULL);
    animatedSpriteDestroy(&gfTitle);
    animatedSpriteDestroy(&logoTitle);
    animatedSpriteDestroy(&pressTitle);
    state_free_sprites();
}
