#ifndef RENDER_H
#define RENDER_H

#include <vulkan/vulkan.h>
#include "sprite.h"

void recordCommands(VkCommandBuffer* pCmdBuf, uint32_t index, VkFramebuffer* pFramebuffer, sprite* sprites, uint32_t spriteCount);
void render(VkFramebuffer* pSwapchainFramebuffers, sprite* sprites, uint32_t spriteCount);

#endif
