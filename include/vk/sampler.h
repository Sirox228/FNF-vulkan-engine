#ifndef SAMPLER_H
#define SAMPLER_H

#include <vulkan/vulkan.h>

void createSampler(VkFilter minFilter, VkFilter magFilter, VkSampler* pSampler);

#endif
