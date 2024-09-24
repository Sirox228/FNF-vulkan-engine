#ifndef RENDER_PASS_H
#define RENDER_PASS_H

#include <vulkan/vulkan.h>

void createAttachmentDescription(VkFormat format, VkSampleCountFlagBits samples, VkAttachmentLoadOp loadOp, VkAttachmentStoreOp storeOp, VkAttachmentLoadOp stencilLoadOp, VkAttachmentStoreOp stencilStoreOp, VkImageLayout initialLayout, VkImageLayout finalLayout, VkAttachmentDescription* pAttachmentDescription);
void createAttachmentReference(uint32_t attachment, VkImageLayout layout, VkAttachmentReference* pAttachmentReference);
void createSubpassDescription(VkPipelineBindPoint pipelineBindPoint, uint32_t colorAttachmentCount, VkAttachmentReference* pColorAttachments, VkAttachmentReference* pResolveAttachments, VkAttachmentReference* pDepthStencilAttachment, uint32_t inputAttachmentCount, VkAttachmentReference* pInputAttachments, uint32_t preserveAttachmentCount, uint32_t* pPreserveAttachments, VkSubpassDescription* pSubpassDescription);
void createSubpassDependency(uint32_t srcSubpass, uint32_t dstSubpass, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkAccessFlags srcAccessMask, VkAccessFlags dstAccessMask, VkDependencyFlags dependencyFlags, VkSubpassDependency* pSubpassDependency);
void createRenderPass(uint32_t attachmentCount, VkAttachmentDescription* pAttachments, uint32_t subpassCount, VkSubpassDescription* pSubpasses, uint32_t dependencyCount, VkSubpassDependency* pDependencies, VkRenderPass* pRenderPass);

#endif
