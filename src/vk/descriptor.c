#include "holder.h"

void createDescriptorPoolSize(VkDescriptorType type, uint32_t descriptorCount, VkDescriptorPoolSize* pPoolSize) {
    pPoolSize->type = type;
    pPoolSize->descriptorCount = descriptorCount;
}

void createDescriptorSetLayoutBinding(uint32_t binding, VkDescriptorType descriptorType, uint32_t descriptorCount, VkShaderStageFlags stageFlags, VkDescriptorSetLayoutBinding* pSetLayoutBinding) {
    pSetLayoutBinding->binding = binding;
    pSetLayoutBinding->descriptorType = descriptorType;
    pSetLayoutBinding->descriptorCount = descriptorCount;
    pSetLayoutBinding->stageFlags = stageFlags;
}

void createDescriptorImageInfo(VkSampler sampler, VkImageView imageView, VkImageLayout imageLayout, VkDescriptorImageInfo* pDescriptorImageInfo) {
    pDescriptorImageInfo->sampler = sampler;
    pDescriptorImageInfo->imageView = imageView;
    pDescriptorImageInfo->imageLayout = imageLayout;
}

void createDescriptorBufferInfo(VkBuffer buffer, VkDeviceSize offset, VkDeviceSize range, VkDescriptorBufferInfo* pDescriptorBufferInfo) {
    pDescriptorBufferInfo->buffer = buffer;
    pDescriptorBufferInfo->offset = offset;
    pDescriptorBufferInfo->range = range;
}

void createWriteDescriptorSet(VkDescriptorSet dstSet, uint32_t dstBinding, uint32_t descriptorCount, VkDescriptorType descriptorType, VkDescriptorImageInfo* pImageInfo, VkDescriptorBufferInfo* pBufferInfo, VkWriteDescriptorSet* pWriteDescriptorSet) {
    pWriteDescriptorSet->sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    pWriteDescriptorSet->dstSet = dstSet;
    pWriteDescriptorSet->dstBinding = dstBinding;
    pWriteDescriptorSet->descriptorCount = descriptorCount;
    pWriteDescriptorSet->descriptorType = descriptorType;
    pWriteDescriptorSet->pImageInfo = pImageInfo;
    pWriteDescriptorSet->pBufferInfo = pBufferInfo;
}

void createDescriptorPool(uint32_t maxSets, VkDescriptorPoolSize pPoolSizes[], uint32_t poolSizeCount, VkDescriptorPool* pPool) {
    VkDescriptorPoolCreateInfo poolInfo = {};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.maxSets = maxSets;
    poolInfo.poolSizeCount = poolSizeCount;
    poolInfo.pPoolSizes = pPoolSizes;

    if (vkCreateDescriptorPool(device, &poolInfo, NULL, pPool) != VK_SUCCESS) {
        printf("failed to create descriptor pool\n");
        exit(0);
    }
}

void createDescriptorSetLayout(VkDescriptorSetLayoutBinding pBindings[], uint32_t bindingCount, VkDescriptorSetLayout* pSetLayout) {
    VkDescriptorSetLayoutCreateInfo setLayoutInfo = {};
    setLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    setLayoutInfo.bindingCount = bindingCount;
    setLayoutInfo.pBindings = pBindings;

    if (vkCreateDescriptorSetLayout(device, &setLayoutInfo, NULL, pSetLayout) != VK_SUCCESS) {
        printf("failed to create descriptor set layout\n");
        exit(0);
    }
}

void createDescriptorSets(VkDescriptorPool descriptorPool, uint32_t descriptorSetCount, VkDescriptorSetLayout pSetLayouts[], VkDescriptorSet pDescriptorSets[]) {
    VkDescriptorSetAllocateInfo setsAllocateInfo = {};
    setsAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    setsAllocateInfo.descriptorPool = descriptorPool;
    setsAllocateInfo.descriptorSetCount = descriptorSetCount;
    setsAllocateInfo.pSetLayouts = pSetLayouts;

    if (vkAllocateDescriptorSets(device, &setsAllocateInfo, pDescriptorSets) != VK_SUCCESS) {
        printf("failed to allocate descriptor sets\n");
        exit(0);
    }
}
