#include "holder.h"
#include <stdio.h>
#include <stdlib.h>

void createAttachmentDescription(VkFormat format, VkSampleCountFlagBits samples, VkAttachmentLoadOp loadOp, VkAttachmentStoreOp storeOp, VkAttachmentLoadOp stencilLoadOp, VkAttachmentStoreOp stencilStoreOp, VkImageLayout initialLayout, VkImageLayout finalLayout, VkAttachmentDescription* pAttachmentDescription) {
    pAttachmentDescription->format = format;
    pAttachmentDescription->samples = samples;
    pAttachmentDescription->loadOp = loadOp;
    pAttachmentDescription->storeOp = storeOp;
    pAttachmentDescription->stencilLoadOp = stencilLoadOp;
    pAttachmentDescription->stencilStoreOp = stencilStoreOp;
    pAttachmentDescription->initialLayout = initialLayout;
    pAttachmentDescription->finalLayout = finalLayout;
}

void createAttachmentReference(uint32_t attachment, VkImageLayout layout, VkAttachmentReference* pAttachmentReference) {
    pAttachmentReference->attachment = attachment;
    pAttachmentReference->layout = layout;
} 

void createSubpassDescription(VkPipelineBindPoint pipelineBindPoint, uint32_t colorAttachmentCount, VkAttachmentReference* pColorAttachments, VkAttachmentReference* pResolveAttachments, VkAttachmentReference* pDepthStencilAttachment, uint32_t inputAttachmentCount, VkAttachmentReference* pInputAttachments, uint32_t preserveAttachmentCount, uint32_t* pPreserveAttachments, VkSubpassDescription* pSubpassDescription) {
    pSubpassDescription->pipelineBindPoint = pipelineBindPoint;
    pSubpassDescription->colorAttachmentCount = colorAttachmentCount;
    pSubpassDescription->pColorAttachments = pColorAttachments;
    pSubpassDescription->pResolveAttachments = pResolveAttachments;
    pSubpassDescription->pDepthStencilAttachment = pDepthStencilAttachment;
    pSubpassDescription->inputAttachmentCount = inputAttachmentCount;
    pSubpassDescription->pInputAttachments = pInputAttachments;
    pSubpassDescription->preserveAttachmentCount = preserveAttachmentCount;
    pSubpassDescription->pPreserveAttachments = pPreserveAttachments;
}

void createSubpassDependency(uint32_t srcSubpass, uint32_t dstSubpass, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkAccessFlags srcAccessMask, VkAccessFlags dstAccessMask, VkDependencyFlags dependencyFlags, VkSubpassDependency* pSubpassDependency) {
    pSubpassDependency->srcSubpass = srcSubpass;
    pSubpassDependency->dstSubpass = dstSubpass;
    pSubpassDependency->srcStageMask = srcStageMask;
    pSubpassDependency->dstStageMask = dstStageMask;
    pSubpassDependency->srcAccessMask = srcAccessMask;
    pSubpassDependency->dstAccessMask = dstAccessMask;
    pSubpassDependency->dependencyFlags = dependencyFlags;
}

void createRenderPass(uint32_t attachmentCount, VkAttachmentDescription* pAttachments, uint32_t subpassCount, VkSubpassDescription* pSubpasses, uint32_t dependencyCount, VkSubpassDependency* pDependencies, VkRenderPass* pRenderPass) {
    VkRenderPassCreateInfo renderPassInfo = {};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = attachmentCount;
    renderPassInfo.pAttachments = pAttachments;
    renderPassInfo.subpassCount = subpassCount;
    renderPassInfo.pSubpasses = pSubpasses;
    renderPassInfo.dependencyCount = dependencyCount;
    renderPassInfo.pDependencies = pDependencies;

    if (vkCreateRenderPass(device, &renderPassInfo, NULL, pRenderPass) != VK_SUCCESS) {
    	printf("failed to create renderpass\n");
        exit(0);
    }
}
