#ifndef RENDER_H
#define RENDER_H

#include <vulkan/vulkan.h>
#include "sprite.h"

void recordCommands(VkCommandBuffer* pCmdBuf, uint32_t index, VkFramebuffer* pFramebuffer, sprite* pSprites, uint32_t spriteCount);
void render(VkFramebuffer* pSwapchainFramebuffers, sprite* pSprites, uint32_t spriteCount);

#endif
