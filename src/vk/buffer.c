#include "holder.h"
#include "vk/vkutil.h"
#include "vk/commandBuffer.h"

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
