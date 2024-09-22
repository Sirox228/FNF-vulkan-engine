#ifndef SYNC_H
#define SYNC_H

#include <vulkan/vulkan.h>

void createSemaphore(VkSemaphore* pSemaphore);
void createFence(uint8_t signaled, VkFence* pFence);

#endif
