#ifndef IMAGE_VIEW_H
#define IMAGE_VIEW_H

#include <vulkan/vulkan.h>

void createImageView(VkImage image, VkFormat format, VkImageViewType viewType, uint32_t baseArrayLayer, uint32_t layerCount, VkImageAspectFlags aspect, VkImageView* pView);

#endif
