#include "holder.h"
#include "vk/vkutil.h"

void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImageLayout layout, VkImageCreateFlags flags, uint32_t arrayLayers, VkImage* pImage, VkDeviceMemory* pImageMemory) {
    VkImageCreateInfo imageInfo = {};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.format = format;
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = arrayLayers;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.tiling = tiling;
    imageInfo.usage = usage;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    imageInfo.initialLayout = layout;
    imageInfo.extent.width = width;
    imageInfo.extent.height = height;
    imageInfo.extent.depth = 1;
    imageInfo.flags = flags;

    if (vkCreateImage(device, &imageInfo, NULL, pImage) != VK_SUCCESS) {
        printf("failed to create image\n");
        exit(0);
    }

    VkMemoryRequirements memReq;
    vkGetImageMemoryRequirements(device, *pImage, &memReq);

    VkMemoryAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memReq.size;
    allocInfo.memoryTypeIndex = findMemoryType(memReq.memoryTypeBits, properties);

    if (vkAllocateMemory(device, &allocInfo, NULL, pImageMemory) != VK_SUCCESS) {
        printf("failed to allocate image memory\n");
        exit(0);
    }

    vkBindImageMemory(device, *pImage, *pImageMemory, 0);
}
