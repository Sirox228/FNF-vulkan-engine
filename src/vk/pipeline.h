#ifndef PIPELINE_H
#define PIPELINE_H

#include <vulkan/vulkan.h>

void createPipelineLayout(VkPipelineLayout* pPipelineLayout, VkDescriptorSetLayout* pSetLayouts, uint32_t setLayoutCount, VkPushConstantRange* pPushConstantRanges, uint32_t pushConstantRangeCount);

void fillDefaultStates();

void createGraphicsPipeline(VkPipeline* pPipeline, VkPipelineLayout sourceLayout, VkRenderPass sourceRenderPass, char* vertPath, char* fragPath, VkPipelineVertexInputStateCreateInfo* vertexInputStateInfo, VkPipelineInputAssemblyStateCreateInfo* inputAssemblyStateInfo, VkPipelineDepthStencilStateCreateInfo* depthStencilStateInfo, VkPipelineViewportStateCreateInfo* viewportStateInfo, VkPipelineRasterizationStateCreateInfo* rasterizationStateInfo, VkPipelineMultisampleStateCreateInfo* multisampleStateInfo, VkPipelineColorBlendStateCreateInfo* blendStateInfo, VkPipelineDynamicStateCreateInfo* dynamicStateInfo);
void createComputePipeline(VkPipeline* pPipeline, VkPipelineLayout pipelineLayout, char* compPath);

#define createGraphicsPipelineDefault(pPipeline, sourceLayout, sourceRenderPass, vertPath, fragPath, vertInpState) createGraphicsPipeline(pPipeline, sourceLayout, sourceRenderPass, vertPath, fragPath, vertInpState, &defaultInputAssemblyStateInfo, &defaultDepthStencilStateInfo, &defaultViewportStateInfo, &defaultRasterizationStateInfo, &defaultMultisampleStateInfo, &defaultBlendStateInfo, NULL)

#endif
