#ifndef SHADER_H
#define SHADER_H

#include <vulkan/vulkan.h>

void createShaderModule(char* pCode, uint32_t codeSize, VkShaderModule* pShaderModule);

#endif
