#include "holder.h"
#include <stdio.h>
#include <stdlib.h>

void createCommandPool(VkCommandPool* pPool, VkCommandPoolCreateFlagBits flags) {
    VkCommandPoolCreateInfo cmdPoolInfo = {};
    cmdPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    cmdPoolInfo.flags = flags;
    cmdPoolInfo.queueFamilyIndex = queueFamilyIndex;

    if (vkCreateCommandPool(device, &cmdPoolInfo, NULL, pPool) != VK_SUCCESS) {
        printf("failed to create command pool\n");
        exit(0);
    }
}
