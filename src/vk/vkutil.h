#ifndef VK_UTIL_H
#define VK_UTIL_H

#include <vulkan/vulkan.h>

VkFormat findFormat(VkFormat* formats, uint32_t formatCount, VkImageTiling tiling, VkFormatFeatureFlags features);
uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
void createImageView(VkImage image, VkFormat format, VkImageViewType viewType, uint32_t baseArrayLayer, uint32_t layerCount, VkImageAspectFlags aspect, VkImageView* pView);
void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer* pBuffer, VkDeviceMemory* pBufferMemory);
void copyBuffer(VkBuffer src, VkBuffer dst, VkDeviceSize size);
void createAndFillBufferFromStaging(VkDeviceSize size, void* data, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer* pBuffer, VkDeviceMemory* pBufferMemory);
void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImageLayout layout, VkImageCreateFlags flags, uint32_t arrayLayers, VkImage* pImage, VkDeviceMemory* pImageMemory);
void setImageMemoryBarrier(VkCommandBuffer cmdBuf, VkImageLayout oldLayout, VkImageLayout newLayout, VkPipelineStageFlags srcStage, VkPipelineStageFlags dstStage, VkAccessFlags srcAccessMask, VkAccessFlags dstAccessMask, VkImage image, VkImageAspectFlagBits aspectMask, uint32_t baseArrayLayer, uint32_t layerCount);
//void createCubemap(char* path, VkImage* pImage, VkDeviceMemory* pImageMemory, VkImageView* pImageView);
void createImageTexture(char* path, VkImage* pImage, VkDeviceMemory* pImageMemory, VkImageView* pImageView, int32_t* pWidth, int32_t* pHeight, int32_t* pChannels);

#endif
