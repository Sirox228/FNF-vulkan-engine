#include "holder.h"
#include "vk/shader.h"
#include "vk/vertex.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <vulkan/vulkan.h>

void createPipelineLayout(VkDescriptorSetLayout* pSetLayouts, uint32_t setLayoutCount, VkPushConstantRange* pPushConstantRanges, uint32_t pushConstantRangeCount, VkPipelineLayout* pPipelineLayout) {
    VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = setLayoutCount;
    pipelineLayoutInfo.pSetLayouts = pSetLayouts;
    pipelineLayoutInfo.pushConstantRangeCount = pushConstantRangeCount;
    pipelineLayoutInfo.pPushConstantRanges = pPushConstantRanges;

    if (vkCreatePipelineLayout(device, &pipelineLayoutInfo, NULL, pPipelineLayout) != VK_SUCCESS) {
        printf("failed to create pipeline layout\n");
        exit(0);
    }
}

void fillDefaultStates() {
    defaultFragStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    defaultFragStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    defaultFragStageInfo.module = NULL;
    defaultFragStageInfo.pName = "main";

    defaultVertStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    defaultVertStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    defaultVertStageInfo.module = NULL;
    defaultVertStageInfo.pName = "main";

    defaultCompStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    defaultCompStageInfo.stage = VK_SHADER_STAGE_COMPUTE_BIT;
    defaultCompStageInfo.module = NULL;
    defaultCompStageInfo.pName = "main";

    defaultTescStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    defaultTescStageInfo.stage = VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
    defaultTescStageInfo.module = NULL;
    defaultTescStageInfo.pName = "main";

    defaultTeseStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    defaultTeseStageInfo.stage = VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
    defaultTeseStageInfo.module = NULL;
    defaultTeseStageInfo.pName = "main";

    defaultVertInpBindDesc.binding = 0;
    defaultVertInpBindDesc.stride = sizeof(vertex);
    defaultVertInpBindDesc.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    defaultVertInpAttrDesc.location = 0;
    defaultVertInpAttrDesc.binding = 0;
    defaultVertInpAttrDesc.format = VK_FORMAT_R32G32_SFLOAT;
    defaultVertInpAttrDesc.offset = offsetof(vertex, pos);

    defaultVertexInputStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    defaultVertexInputStateInfo.vertexBindingDescriptionCount = 1;
    defaultVertexInputStateInfo.pVertexBindingDescriptions = &defaultVertInpBindDesc;
    defaultVertexInputStateInfo.vertexAttributeDescriptionCount = 1;
    defaultVertexInputStateInfo.pVertexAttributeDescriptions = &defaultVertInpAttrDesc;

    defaultInputAssemblyStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    defaultInputAssemblyStateInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    defaultInputAssemblyStateInfo.primitiveRestartEnable = VK_FALSE;

    defaultTessellationInputAssemblyStateInfo = defaultInputAssemblyStateInfo;
    defaultTessellationInputAssemblyStateInfo.topology = VK_PRIMITIVE_TOPOLOGY_PATCH_LIST;

    defaultTessellationStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO;
    defaultTessellationStateInfo.patchControlPoints = 3;

    defaultDepthStencilStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    defaultDepthStencilStateInfo.depthTestEnable = VK_FALSE;
    defaultDepthStencilStateInfo.depthWriteEnable = VK_FALSE;
    defaultDepthStencilStateInfo.depthCompareOp = VK_COMPARE_OP_GREATER;
    defaultDepthStencilStateInfo.depthBoundsTestEnable = VK_FALSE;
    defaultDepthStencilStateInfo.minDepthBounds = 0.0f;
    defaultDepthStencilStateInfo.maxDepthBounds = 1.0f;
    defaultDepthStencilStateInfo.stencilTestEnable = VK_FALSE;
    defaultDepthStencilStateInfo.front = (VkStencilOpState){};
    defaultDepthStencilStateInfo.back = (VkStencilOpState){};

    defaultViewport.x = 0;
    defaultViewport.y = 0;
    defaultViewport.width = swapchainExtent.width;
    defaultViewport.height = swapchainExtent.height;
    defaultViewport.minDepth = 0.0f;
    defaultViewport.maxDepth = 1.0f;

    defaultScissor.offset = (VkOffset2D){0, 0};
    defaultScissor.extent = swapchainExtent;

    defaultViewportStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    defaultViewportStateInfo.viewportCount = 1;
    defaultViewportStateInfo.pViewports = &defaultViewport;
    defaultViewportStateInfo.scissorCount = 1;
    defaultViewportStateInfo.pScissors = &defaultScissor;

    defaultRasterizationStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    defaultRasterizationStateInfo.depthClampEnable = VK_FALSE;
    defaultRasterizationStateInfo.rasterizerDiscardEnable = VK_FALSE;
    defaultRasterizationStateInfo.polygonMode = VK_POLYGON_MODE_FILL;
    defaultRasterizationStateInfo.cullMode = VK_CULL_MODE_NONE;
    defaultRasterizationStateInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
    defaultRasterizationStateInfo.depthBiasEnable = VK_FALSE;
    defaultRasterizationStateInfo.lineWidth = 1.0f;

    defaultMultisampleStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    defaultMultisampleStateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    defaultMultisampleStateInfo.sampleShadingEnable = VK_FALSE;
    defaultMultisampleStateInfo.minSampleShading = 1.0f;
    defaultMultisampleStateInfo.pSampleMask = NULL;
    defaultMultisampleStateInfo.alphaToCoverageEnable = VK_FALSE;
    defaultMultisampleStateInfo.alphaToOneEnable = VK_FALSE;

    defaultAlphaBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    defaultAlphaBlendAttachment.blendEnable = VK_TRUE;
    defaultAlphaBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
    defaultAlphaBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    defaultAlphaBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
    defaultAlphaBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    defaultAlphaBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    defaultAlphaBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

    defaultBlendStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    defaultBlendStateInfo.logicOpEnable = VK_FALSE;
    defaultBlendStateInfo.logicOp = VK_LOGIC_OP_COPY;
    defaultBlendStateInfo.attachmentCount = 1;
    defaultBlendStateInfo.pAttachments = &defaultAlphaBlendAttachment;
    defaultBlendStateInfo.blendConstants[0] = 0.0f;
    defaultBlendStateInfo.blendConstants[1] = 0.0f;
    defaultBlendStateInfo.blendConstants[2] = 0.0f;
    defaultBlendStateInfo.blendConstants[3] = 0.0f;
}

void createGraphicsPipeline(VkPipelineLayout sourceLayout, VkRenderPass sourceRenderPass, uint32_t subpass, char* vertPath, char* fragPath, char* tescPath, char* tesePath, VkPipelineVertexInputStateCreateInfo* vertexInputStateInfo, VkPipelineInputAssemblyStateCreateInfo* inputAssemblyStateInfo, VkPipelineTessellationStateCreateInfo* tessellationStateInfo, VkPipelineDepthStencilStateCreateInfo* depthStencilStateInfo, VkPipelineViewportStateCreateInfo* viewportStateInfo, VkPipelineRasterizationStateCreateInfo* rasterizationStateInfo, VkPipelineMultisampleStateCreateInfo* multisampleStateInfo, VkPipelineColorBlendStateCreateInfo* blendStateInfo, VkPipelineDynamicStateCreateInfo* dynamicStateInfo, VkPipeline* pPipeline) {
    uint32_t vertCodeSize, fragCodeSize, tescCodeSize, teseCodeSize;
    char* vertCode, *fragCode, *tescCode, *teseCode;
    VkShaderModule vert = NULL, frag = NULL, tesc = NULL, tese = NULL;
    VkPipelineShaderStageCreateInfo stages[4] = {};
    uint32_t stageCount = 0;

    #define parseStage(stagePath, stageCode, stageCodeSize, stageModule, stageDefault) \
    if (stagePath != NULL) { \
        getFileContent(stagePath, &stageCodeSize, &stageCode); \
        createShaderModule(stageCode, stageCodeSize, &stageModule); \
        free(stageCode); \
        stages[stageCount] = stageDefault; \
        stages[stageCount].module = stageModule; \
        stageCount++; \
    }

    parseStage(vertPath, vertCode, vertCodeSize, vert, defaultVertStageInfo)
    parseStage(fragPath, fragCode, fragCodeSize, frag, defaultFragStageInfo)
    parseStage(tescPath, tescCode, tescCodeSize, tesc, defaultTescStageInfo)
    parseStage(tesePath, teseCode, teseCodeSize, tese, defaultTeseStageInfo)

    VkGraphicsPipelineCreateInfo pipelineInfo = {};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = stageCount;
    pipelineInfo.pStages = stages;
    pipelineInfo.pVertexInputState = vertexInputStateInfo;
    pipelineInfo.pInputAssemblyState = inputAssemblyStateInfo;
    pipelineInfo.pTessellationState = tessellationStateInfo;
    pipelineInfo.pViewportState = viewportStateInfo;
    pipelineInfo.pRasterizationState = rasterizationStateInfo;
    pipelineInfo.pMultisampleState = multisampleStateInfo;
    pipelineInfo.pDepthStencilState = depthStencilStateInfo;
    pipelineInfo.pColorBlendState = blendStateInfo;
    pipelineInfo.pDynamicState = dynamicStateInfo;
    pipelineInfo.layout = sourceLayout;
    pipelineInfo.renderPass = sourceRenderPass;
    pipelineInfo.subpass = subpass;
    pipelineInfo.basePipelineHandle = NULL;

    if (vkCreateGraphicsPipelines(device, NULL, 1, &pipelineInfo, NULL, pPipeline) != VK_SUCCESS) {
        printf("failed to create graphics pipeline\n");
        exit(0);
    }

    if (frag != NULL) vkDestroyShaderModule(device, frag, NULL);
    if (vert != NULL) vkDestroyShaderModule(device, vert, NULL);
    if (tesc != NULL) vkDestroyShaderModule(device, tesc, NULL);
    if (tese != NULL) vkDestroyShaderModule(device, tese, NULL);
}

void createComputePipeline(VkPipelineLayout pipelineLayout, char* compPath, VkPipeline* pPipeline) {
    uint32_t compCodeSize;
    char* compCode;
    VkShaderModule comp;

    getFileContent(compPath, &compCodeSize, &compCode);
    createShaderModule(compCode, compCodeSize, &comp);
    free(compCode);

    VkPipelineShaderStageCreateInfo compStageInfo = defaultCompStageInfo;
    compStageInfo.module = comp;

    VkComputePipelineCreateInfo pipelineInfo = {};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
    pipelineInfo.stage = compStageInfo;
    pipelineInfo.layout = pipelineLayout;

    if (vkCreateComputePipelines(device, NULL, 1, &pipelineInfo, NULL, pPipeline) != VK_SUCCESS) {
        printf("failed to create compute pipeline\n");
        exit(0);
    }

    vkDestroyShaderModule(device, comp, NULL);
}
