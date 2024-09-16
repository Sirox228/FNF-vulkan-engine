#include "holder.h"
#include "commandBuffer.h"
#include "stbi/stb_image.h"
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
    for (uint32_t i = 0; i < physicalDeviceMemProps.memoryTypeCount; i++) {
        if (typeFilter & (1 << i) && (physicalDeviceMemProps.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }
    printf("failed to find required memory type. type %d properties %d\n", typeFilter, properties);
    exit(0);
}

void createImageView(VkImage image, VkFormat format, VkImageViewType viewType, uint32_t baseArrayLayer, uint32_t layerCount, VkImageAspectFlags aspect, VkImageView* pView) {
    VkImageViewCreateInfo imageViewInfo = {};
    imageViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    imageViewInfo.image = image;
    imageViewInfo.viewType = viewType;
    imageViewInfo.format = format;
    imageViewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    imageViewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    imageViewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    imageViewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
    imageViewInfo.subresourceRange.aspectMask = aspect;
    imageViewInfo.subresourceRange.baseMipLevel = 0;
    imageViewInfo.subresourceRange.levelCount = 1;
    imageViewInfo.subresourceRange.baseArrayLayer = baseArrayLayer;
    imageViewInfo.subresourceRange.layerCount = layerCount;

    if (vkCreateImageView(device, &imageViewInfo, NULL, pView) != VK_SUCCESS) {
        printf("failed to create image view\n");
        exit(0);
    }
}

void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer* pBuffer, VkDeviceMemory* pBufferMemory) {
    VkBufferCreateInfo bufferInfo = {};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(device, &bufferInfo, NULL, pBuffer) != VK_SUCCESS) {
        printf("failed to create buffer\n");
        exit(0);
    }

    VkMemoryRequirements memReq;
    vkGetBufferMemoryRequirements(device, *pBuffer, &memReq);

    VkMemoryAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memReq.size;
    allocInfo.memoryTypeIndex = findMemoryType(memReq.memoryTypeBits, properties);

    if (vkAllocateMemory(device, &allocInfo, NULL, pBufferMemory) != VK_SUCCESS) {
        printf("failed to allocate buffer memory\n");
        exit(0);
    }

    vkBindBufferMemory(device, *pBuffer, *pBufferMemory, 0);
}

void copyBuffer(VkBuffer src, VkBuffer dst, VkDeviceSize size) {
    VkCommandBuffer cmdBuf = getSingleTimeCmdBuf();

    VkBufferCopy copyInfo = {};
    copyInfo.srcOffset = 0;
    copyInfo.dstOffset = 0;
    copyInfo.size = size;

    vkCmdCopyBuffer(cmdBuf, src, dst, 1, &copyInfo);

    submitSingleTimeCmdBuf(&cmdBuf);
}

void createAndFillBufferFromStaging(VkDeviceSize size, void* data, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer* pBuffer, VkDeviceMemory* pBufferMemory) {
    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMem;

    createBuffer(size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &stagingBuffer, &stagingBufferMem);

    void* stagingBufferMemMapped;
    vkMapMemory(device, stagingBufferMem, 0, size, 0, &stagingBufferMemMapped);
    memcpy(stagingBufferMemMapped, data, size);
    vkUnmapMemory(device, stagingBufferMem);

    createBuffer(size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | usage, properties, pBuffer, pBufferMemory);

    copyBuffer(stagingBuffer, *pBuffer, size);

    vkDestroyBuffer(device, stagingBuffer, NULL);
    vkFreeMemory(device, stagingBufferMem, NULL);
}

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

/*void createCubemap(char* path, VkImage* pImage, VkDeviceMemory* pImageMemory, VkImageView* pImageView) {
    ktxTexture* tex;
    if (ktxTexture_CreateFromNamedFile(path, KTX_TEXTURE_CREATE_LOAD_IMAGE_DATA_BIT, &tex) != KTX_SUCCESS) {
        printf("failed to read cubemap: %s\n", path);
        exit(0);
    }

    ktx_uint8_t* texData = ktxTexture_GetData(tex);
    ktx_size_t texSize = ktxTexture_GetDataSize(tex);

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMem;
    createBuffer(texSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &stagingBuffer, &stagingBufferMem);

    void* stagingBufferMemMapped;
    vkMapMemory(device, stagingBufferMem, 0, texSize, 0, &stagingBufferMemMapped);
    memcpy(stagingBufferMemMapped, texData, texSize);
    vkUnmapMemory(device, stagingBufferMem);

    ktxTexture_Destroy(tex);

    createImage(tex->baseWidth, tex->baseHeight, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT, 6, pImage, pImageMemory);

    VkBufferImageCopy copyCmds[6] = {{}, {}, {}, {}, {}, {}};

    for (uint32_t i = 0; i < 6; i++) {
        ktx_size_t offset;
        if (ktxTexture_GetImageOffset(tex, 0, 0, i, &offset) != KTX_SUCCESS) {
            printf("failed to get offset for face %d of cubemap: %s\n", i, path);
            exit(0);
        }

        VkBufferImageCopy copyCmd = {};
        copyCmd.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        copyCmd.imageSubresource.mipLevel = 0;
        copyCmd.imageSubresource.baseArrayLayer = i;
        copyCmd.imageSubresource.layerCount = 1;
        copyCmd.imageExtent.width = tex->baseWidth;
        copyCmd.imageExtent.height = tex->baseHeight;
        copyCmd.imageExtent.depth = 1;
        copyCmd.bufferOffset = offset;

        copyCmds[i] = copyCmd;
    }

    VkCommandBuffer cmdBuf = getSingleTimeCmdBuf();

    setImageMemoryBarrier(cmdBuf, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, VK_ACCESS_TRANSFER_WRITE_BIT, *pImage, VK_IMAGE_ASPECT_COLOR_BIT, 0, 6);
    vkCmdCopyBufferToImage(cmdBuf, stagingBuffer, *pImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 6, copyCmds);
    setImageMemoryBarrier(cmdBuf, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_SHADER_READ_BIT, *pImage, VK_IMAGE_ASPECT_COLOR_BIT, 0, 6);

    submitSingleTimeCmdBuf(&cmdBuf);

    vkDestroyBuffer(device, stagingBuffer, NULL);
    vkFreeMemory(device, stagingBufferMem, NULL);

    createImageView(*pImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_VIEW_TYPE_CUBE, 0, 6, VK_IMAGE_ASPECT_COLOR_BIT, pImageView);
}*/

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
