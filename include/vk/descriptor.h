#ifndef DESCRIPTOR_H
#define DESCRIPTOR_H

#include <vulkan/vulkan.h>

void createDescriptorPoolSize(VkDescriptorType type, uint32_t descriptorCount, VkDescriptorPoolSize* pPoolSize);
void createDescriptorSetLayoutBinding(uint32_t binding, VkDescriptorType descriptorType, uint32_t descriptorCount, VkShaderStageFlags stageFlags, VkDescriptorSetLayoutBinding* pSetLayoutBinding);
void createDescriptorImageInfo(VkSampler sampler, VkImageView imageView, VkImageLayout imageLayout, VkDescriptorImageInfo* pDescriptorImageInfo);
void createDescriptorBufferInfo(VkBuffer buffer, VkDeviceSize offset, VkDeviceSize range, VkDescriptorBufferInfo* pDescriptorBufferInfo);
void createWriteDescriptorSet(VkDescriptorSet dstSet, uint32_t dstBinding, uint32_t descriptorCount, VkDescriptorType descriptorType, VkDescriptorImageInfo* pImageInfo, VkDescriptorBufferInfo* pBufferInfo, VkWriteDescriptorSet* pWriteDescriptorSet);
void createDescriptorPool(uint32_t maxSets, VkDescriptorPoolSize pPoolSizes[], uint32_t poolSizeCount, VkDescriptorPool* pPool);
void createDescriptorSetLayout(VkDescriptorSetLayoutBinding pBindings[], uint32_t bindingCount, VkDescriptorSetLayout* pSetLayout);
void createDescriptorSets(VkDescriptorPool descriptorPool, uint32_t descriptorSetCount, VkDescriptorSetLayout pSetLayouts[], VkDescriptorSet pDescriptorSets[]);

#endif
