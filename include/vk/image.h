#ifndef IMAGE_H
#define IMAGE_H

#include <vulkan/vulkan.h>

void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImageLayout layout, VkImageCreateFlags flags, uint32_t arrayLayers, VkImage* pImage, VkDeviceMemory* pImageMemory);

#endif
