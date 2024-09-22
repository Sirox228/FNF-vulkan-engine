#ifndef COMMAND_BUFFER_H
#define COMMAND_BUFFER_H

#include <vulkan/vulkan.h>

void createCommandBuffers(uint32_t commandBufferCount, VkCommandPool commandPool, VkCommandBuffer* pCommandBuffers);
VkCommandBuffer getSingleTimeCmdBuf();
void submitSingleTimeCmdBuf(VkCommandBuffer* pCommandBuffer);

#endif
