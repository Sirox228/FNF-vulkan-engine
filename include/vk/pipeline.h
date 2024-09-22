#ifndef PIPELINE_H
#define PIPELINE_H

#include <vulkan/vulkan.h>

void createPipelineLayout(VkDescriptorSetLayout* pSetLayouts, uint32_t setLayoutCount, VkPushConstantRange* pPushConstantRanges, uint32_t pushConstantRangeCount, VkPipelineLayout* pPipelineLayout);

void fillDefaultStates();

void createGraphicsPipeline(VkPipelineLayout sourceLayout, VkRenderPass sourceRenderPass, uint32_t subpass, char* vertPath, char* fragPath, char* tescPath, char* tesePath, VkPipelineVertexInputStateCreateInfo* vertexInputStateInfo, VkPipelineInputAssemblyStateCreateInfo* inputAssemblyStateInfo, VkPipelineTessellationStateCreateInfo* tessellationStateInfo, VkPipelineDepthStencilStateCreateInfo* depthStencilStateInfo, VkPipelineViewportStateCreateInfo* viewportStateInfo, VkPipelineRasterizationStateCreateInfo* rasterizationStateInfo, VkPipelineMultisampleStateCreateInfo* multisampleStateInfo, VkPipelineColorBlendStateCreateInfo* blendStateInfo, VkPipelineDynamicStateCreateInfo* dynamicStateInfo, VkPipeline* pPipeline);
void createComputePipeline(VkPipelineLayout pipelineLayout, char* compPath, VkPipeline* pPipeline);

#define createGraphicsPipelineDefault(sourceLayout, sourceRenderPass, subpass, vertPath, fragPath, tescPath, tesePath, vertInpState, tessellationState, pPipeline) createGraphicsPipeline(sourceLayout, sourceRenderPass, subpass, vertPath, fragPath, tescPath, tesePath, vertInpState, &defaultInputAssemblyStateInfo, &defaultTessellationStateInfo, &defaultDepthStencilStateInfo, &defaultViewportStateInfo, &defaultRasterizationStateInfo, &defaultMultisampleStateInfo, &defaultBlendStateInfo, NULL, pPipeline)

#endif
