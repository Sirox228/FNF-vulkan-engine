#ifndef VK_UTIL_H
#define VK_UTIL_H

#include <vulkan/vulkan.h>

VkFormat findFormat(VkFormat* formats, uint32_t formatCount, VkImageTiling tiling, VkFormatFeatureFlags features);
uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
void setImageMemoryBarrier(VkCommandBuffer cmdBuf, VkImageLayout oldLayout, VkImageLayout newLayout, VkPipelineStageFlags srcStage, VkPipelineStageFlags dstStage, VkAccessFlags srcAccessMask, VkAccessFlags dstAccessMask, VkImage image, VkImageAspectFlagBits aspectMask, uint32_t baseArrayLayer, uint32_t layerCount);
void createImageTexture(char* path, VkImage* pImage, VkDeviceMemory* pImageMemory, VkImageView* pImageView, int32_t* pWidth, int32_t* pHeight, int32_t* pChannels);

#endif
