#ifndef PIPELINE_H
#define PIPELINE_H

#include <vulkan/vulkan.h>

void createPipelineLayout(VkPipelineLayout* pPipelineLayout, VkDescriptorSetLayout* pSetLayouts, uint32_t setLayoutCount, VkPushConstantRange* pPushConstantRanges, uint32_t pushConstantRangeCount);

void fillDefaultStates();

void createGraphicsPipeline(VkPipeline* pPipeline, VkPipelineLayout sourceLayout, VkRenderPass sourceRenderPass, uint32_t subpass, char* vertPath, char* fragPath, char* tescPath, char* tesePath, VkPipelineVertexInputStateCreateInfo* vertexInputStateInfo, VkPipelineInputAssemblyStateCreateInfo* inputAssemblyStateInfo, VkPipelineTessellationStateCreateInfo* tessellationStateInfo, VkPipelineDepthStencilStateCreateInfo* depthStencilStateInfo, VkPipelineViewportStateCreateInfo* viewportStateInfo, VkPipelineRasterizationStateCreateInfo* rasterizationStateInfo, VkPipelineMultisampleStateCreateInfo* multisampleStateInfo, VkPipelineColorBlendStateCreateInfo* blendStateInfo, VkPipelineDynamicStateCreateInfo* dynamicStateInfo);
void createComputePipeline(VkPipeline* pPipeline, VkPipelineLayout pipelineLayout, char* compPath);

#define createGraphicsPipelineDefault(pPipeline, sourceLayout, sourceRenderPass, subpass, vertPath, fragPath, tescPath, tesePath, vertInpState, tessellationState) createGraphicsPipeline(pPipeline, sourceLayout, sourceRenderPass, subpass, vertPath, fragPath, tescPath, tesePath, vertInpState, &defaultInputAssemblyStateInfo, &defaultTessellationStateInfo, &defaultDepthStencilStateInfo, &defaultViewportStateInfo, &defaultRasterizationStateInfo, &defaultMultisampleStateInfo, &defaultBlendStateInfo, NULL)

#endif
