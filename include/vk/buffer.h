#ifndef BUFFER_H
#define BUFFER_H

#include <vulkan/vulkan.h>

void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer* pBuffer, VkDeviceMemory* pBufferMemory);
void copyBuffer(VkBuffer src, VkBuffer dst, VkDeviceSize size);
void createAndFillBufferFromStaging(VkDeviceSize size, void* data, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer* pBuffer, VkDeviceMemory* pBufferMemory);

#endif
