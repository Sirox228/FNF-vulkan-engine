#include "holder.h"
#include <stdio.h>
#include <stdlib.h>

void createSemaphore(VkSemaphore* pSemaphore) {
    VkSemaphoreCreateInfo semaphoreInfo = {};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    if (vkCreateSemaphore(device, &semaphoreInfo, NULL, pSemaphore) != VK_SUCCESS) {
        printf("failed to create semaphore\n");
        exit(0);
    }
}

void createFence(uint8_t signaled, VkFence* pFence) {
    VkFenceCreateInfo fenceInfo = {};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = signaled ? VK_FENCE_CREATE_SIGNALED_BIT : 0;

    if (vkCreateFence(device, &fenceInfo, NULL, pFence) != VK_SUCCESS) {
        printf("failes to create fence\n");
        exit(0);
    }
}
