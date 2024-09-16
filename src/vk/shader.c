#include "holder.h"
#include <stdio.h>
#include <stdlib.h>

void createShaderModule(VkShaderModule* pShaderModule, char* pCode, uint32_t codeSize) {
    VkShaderModuleCreateInfo shaderModuleInfo = {};
    shaderModuleInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    shaderModuleInfo.pCode = (uint32_t*)pCode;
    shaderModuleInfo.codeSize = codeSize;

    if (vkCreateShaderModule(device, &shaderModuleInfo, NULL, pShaderModule) != VK_SUCCESS) {
        printf("failed to create shader module\n");
        exit(0);
    }
}
