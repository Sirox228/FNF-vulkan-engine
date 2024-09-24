#include "holder.h"
#include "config.h"
#include <stdio.h>
#include <stdlib.h>

void createCommandBuffers(uint32_t commandBufferCount, VkCommandPool commandPool, VkCommandBuffer* pCommandBuffers) {
    VkCommandBufferAllocateInfo cmdBufInfo = {};
    cmdBufInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    cmdBufInfo.commandPool = commandPool;
    cmdBufInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    cmdBufInfo.commandBufferCount = commandBufferCount;

    if (vkAllocateCommandBuffers(device, &cmdBufInfo, pCommandBuffers) != VK_SUCCESS) {
        printf("failed to allocate swapchain command buffers\n");
        exit(0);
    }
}

VkCommandBuffer getSingleTimeCmdBuf() {
    VkCommandBufferAllocateInfo cmdBufInfo = {};
    cmdBufInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    cmdBufInfo.commandPool = shortCommandPool;
    cmdBufInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    cmdBufInfo.commandBufferCount = 1;

    VkCommandBuffer cmdBuf;
    if (vkAllocateCommandBuffers(device, &cmdBufInfo, &cmdBuf) != VK_SUCCESS) {
        printf("failed to allocate command buffer\n");
        exit(0);
    }

    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    if (vkBeginCommandBuffer(cmdBuf, &beginInfo) != VK_SUCCESS) {
        printf("failed to begin command buffer\n");
        exit(0);
    }

    return cmdBuf;
}

void submitSingleTimeCmdBuf(VkCommandBuffer* pCommandBuffer) {
    vkEndCommandBuffer(*pCommandBuffer);

    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = pCommandBuffer;

    // TODO: make proper synchronization here
    vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE);

    vkQueueWaitIdle(queue);
    vkFreeCommandBuffers(device, shortCommandPool, 1, pCommandBuffer);
}
