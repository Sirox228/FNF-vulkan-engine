#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <vulkan/vulkan.h>

void createFramebuffer(VkImageView* pAttachments, uint32_t attachmentCount, VkRenderPass sourceRenderPass, VkExtent2D dimensions, VkFramebuffer* pFramebuffer);

#endif
