#include "holder.h"
#include <stbi/stb_image.h>
#include "vk/image.h"
#include "vk/imageView.h"
#include "vk/buffer.h"
#include "vk/commandBuffer.h"
#include <stdio.h>
#include <stdlib.h>

VkFormat findFormat(VkFormat* formats, uint32_t formatCount, VkImageTiling tiling, VkFormatFeatureFlags features) {
    for (int i = 0; i < formatCount; i++) {
        VkFormatProperties props;

        vkGetPhysicalDeviceFormatProperties(physicalDevice, formats[i], &props);

        if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features)
            return formats[i];
        else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features)
            return formats[i];
    }
    return formats[0];
}

uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
    for (uint32_t i = 0; i < deviceMemProperties.memoryTypeCount; i++) {
        if (typeFilter & (1 << i) && (deviceMemProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }
    printf("failed to find required memory type. type %d properties %d\n", typeFilter, properties);
    exit(0);
}

void setImageMemoryBarrier(VkCommandBuffer cmdBuf, VkImageLayout oldLayout, VkImageLayout newLayout, VkPipelineStageFlags srcStage, VkPipelineStageFlags dstStage, VkAccessFlags srcAccessMask, VkAccessFlags dstAccessMask, VkImage image, VkImageAspectFlagBits aspectMask, uint32_t baseArrayLayer, uint32_t layerCount) {
    VkImageMemoryBarrier barrier = {};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout = oldLayout;
    barrier.newLayout = newLayout;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image = image;
    barrier.subresourceRange.aspectMask = aspectMask;
    barrier.subresourceRange.levelCount = 1;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.layerCount = layerCount;
    barrier.subresourceRange.baseArrayLayer = baseArrayLayer;

    vkCmdPipelineBarrier(cmdBuf, srcStage, dstStage, 0, 0, NULL, 0, NULL, 1, &barrier);
}


void createImageTexture(char* path, VkImage* pImage, VkDeviceMemory* pImageMemory, VkImageView* pImageView, int32_t* pWidth, int32_t* pHeight, int32_t* pChannels) {
    stbi_uc* texData = stbi_load(path, pWidth, pHeight, pChannels, STBI_rgb_alpha);
    if (!texData) {
        printf("failed to read texture: %s\n", path);
        exit(0);
    }
    VkDeviceSize texSize = *pWidth * *pHeight * 4;

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMem;
    createBuffer(texSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &stagingBuffer, &stagingBufferMem);

    void* stagingBufferMemMapped;
    vkMapMemory(device, stagingBufferMem, 0, texSize, 0, &stagingBufferMemMapped);
    memcpy(stagingBufferMemMapped, texData, texSize);
    vkUnmapMemory(device, stagingBufferMem);

    stbi_image_free(texData);

    createImage(*pWidth, *pHeight, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, VK_IMAGE_LAYOUT_UNDEFINED, 0, 1, pImage, pImageMemory);

    VkBufferImageCopy copyCmd = {};
    copyCmd.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    copyCmd.imageSubresource.mipLevel = 0;
    copyCmd.imageSubresource.baseArrayLayer = 0;
    copyCmd.imageSubresource.layerCount = 1;
    copyCmd.imageExtent.width = *pWidth;
    copyCmd.imageExtent.height = *pHeight;
    copyCmd.imageExtent.depth = 1;

    VkCommandBuffer cmdBuf = getSingleTimeCmdBuf();

    setImageMemoryBarrier(cmdBuf, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, VK_ACCESS_TRANSFER_WRITE_BIT, *pImage, VK_IMAGE_ASPECT_COLOR_BIT, 0, 1);
    vkCmdCopyBufferToImage(cmdBuf, stagingBuffer, *pImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copyCmd);
    setImageMemoryBarrier(cmdBuf, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_SHADER_READ_BIT, *pImage, VK_IMAGE_ASPECT_COLOR_BIT, 0, 1);

    submitSingleTimeCmdBuf(&cmdBuf);

    vkDestroyBuffer(device, stagingBuffer, NULL);
    vkFreeMemory(device, stagingBufferMem, NULL);

    createImageView(*pImage, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_VIEW_TYPE_2D, 0, 1, VK_IMAGE_ASPECT_COLOR_BIT, pImageView);
}
