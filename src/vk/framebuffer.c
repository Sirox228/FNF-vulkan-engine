#include "holder.h"
#include <stdio.h>
#include <stdlib.h>

void createFramebuffer(VkImageView* pAttachments, uint32_t attachmentCount, VkRenderPass sourceRenderPass, VkExtent2D dimensions, VkFramebuffer* pFramebuffer) {
    VkFramebufferCreateInfo framebufInfo = {};
    framebufInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufInfo.renderPass = sourceRenderPass;
    framebufInfo.attachmentCount = attachmentCount;
    framebufInfo.pAttachments = pAttachments;
    framebufInfo.width = dimensions.width;
    framebufInfo.height = dimensions.height;
    framebufInfo.layers = 1;

    if (vkCreateFramebuffer(device, &framebufInfo, NULL, pFramebuffer) != VK_SUCCESS) {
        printf("failed to create framebuffer\n");
        exit(0);
    }
}
