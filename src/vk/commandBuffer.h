#ifndef COMMAND_BUFFER_H
#define COMMAND_BUFFER_H

#include <vulkan/vulkan.h>

void createSwapchainCommandBuffers(VkCommandBuffer* pCommandBuffers, VkCommandPool commandPool);
VkCommandBuffer getSingleTimeCmdBuf();
void submitSingleTimeCmdBuf(VkCommandBuffer* pCommandBuffer);

#endif
