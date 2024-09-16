#ifndef SHADER_H
#define SHADER_H

#include <vulkan/vulkan.h>

void createShaderModule(VkShaderModule* pShaderModule, char* pCode, uint32_t codeSize);

#endif
