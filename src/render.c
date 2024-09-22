#include "holder.h"
#include "sprite.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

void recordCommands(VkCommandBuffer* pCmdBuf, uint32_t index, VkFramebuffer* pFramebuffer, sprite* sprites, uint32_t spriteCount) {
    VkCommandBuffer cmdBuf = *pCmdBuf;

    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if (vkBeginCommandBuffer(cmdBuf, &beginInfo) != VK_SUCCESS) {
        printf("failed to begin command buffer\n");
        exit(0);
    }

    VkRenderPassBeginInfo rpBeginInfo = {};
    rpBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    rpBeginInfo.renderPass = renderPass;
    rpBeginInfo.framebuffer = *pFramebuffer;
    rpBeginInfo.renderArea.offset = (VkOffset2D){0, 0};
    rpBeginInfo.renderArea.extent = swapchainExtent;
    rpBeginInfo.clearValueCount = 1;
    rpBeginInfo.pClearValues = &clearColorValue;

    vkCmdBeginRenderPass(cmdBuf, &rpBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

    VkDeviceSize quadVertexBufferOffsets[1] = {0};
    VkViewport viewport;

    for (uint32_t i = 0; i < spriteCount; i++) {
        if (sprites[i].isAnimated) {
            viewport = sprites[i].viewport;
            viewport.width = sprites[i].atlas.animations[sprites[i].animIndex].frames[sprites[i].animFrame].w * sprites[i].scaleX;
            viewport.height = sprites[i].atlas.animations[sprites[i].animIndex].frames[sprites[i].animFrame].h * sprites[i].scaleY;
            vkCmdSetViewport(cmdBuf, 0, 1, &viewport);
            vkCmdSetScissor(cmdBuf, 0, 1, &sprites[i].scissor);
            vkCmdBindPipeline(cmdBuf, VK_PIPELINE_BIND_POINT_GRAPHICS, animatedSpritePipeline);
            vkCmdBindVertexBuffers(cmdBuf, 0, 1, &quadVertexBuffer, quadVertexBufferOffsets);
            vkCmdBindIndexBuffer(cmdBuf, quadIndexBuffer, 0, VK_INDEX_TYPE_UINT32);
            vkCmdBindDescriptorSets(cmdBuf, VK_PIPELINE_BIND_POINT_GRAPHICS, animatedSpritePipelineLayout, 0, 1, &sprites[i].descriptorSet, 0, NULL);

            uint32_t pc = (sprites[i].framesOffset + sprites[i].animFrame) * 4;
            vkCmdPushConstants(cmdBuf, animatedSpritePipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(uint32_t), &pc);

            vkCmdDrawIndexed(cmdBuf, QUAD_IDX_NUM, 1, 0, 0, 0);
        } else {
            viewport = sprites[i].viewport;
            viewport.width *= sprites[i].scaleX;
            viewport.height *= sprites[i].scaleY;
            vkCmdSetViewport(cmdBuf, 0, 1, &viewport);
            vkCmdSetScissor(cmdBuf, 0, 1, &sprites[i].scissor);
            vkCmdBindPipeline(cmdBuf, VK_PIPELINE_BIND_POINT_GRAPHICS, staticSpritePipeline);
            vkCmdBindVertexBuffers(cmdBuf, 0, 1, &quadVertexBuffer, quadVertexBufferOffsets);
            vkCmdBindIndexBuffer(cmdBuf, quadIndexBuffer, 0, VK_INDEX_TYPE_UINT32);
            vkCmdBindDescriptorSets(cmdBuf, VK_PIPELINE_BIND_POINT_GRAPHICS, staticSpritePipelineLayout, 0, 1, &sprites[i].descriptorSet, 0, NULL);

            vkCmdDrawIndexed(cmdBuf, QUAD_IDX_NUM, 1, 0, 0, 0);
        }
    }

    vkCmdEndRenderPass(cmdBuf);

    if (vkEndCommandBuffer(cmdBuf) != VK_SUCCESS) {
        printf("failed to end command buffer\n");
        exit(0);
    }
}

void render(VkFramebuffer* pSwapchainFramebuffers, sprite* sprites, uint32_t spriteCount) {
    vkWaitForFences(device, 1, &inFlightFences[frame], VK_TRUE, UINT64_MAX);

    uint32_t index;
    vkAcquireNextImageKHR(device, swapchain, UINT64_MAX, imageAvailableSemaphores[frame], NULL, &index);

    vkResetFences(device, 1, &inFlightFences[frame]);

    vkResetCommandBuffer(swapchainCommandBuffers[frame], 0);

    recordCommands(&swapchainCommandBuffers[frame], index, &pSwapchainFramebuffers[index], sprites, spriteCount);

    VkSemaphore waitSems[1] = {imageAvailableSemaphores[frame]};
    VkSemaphore signalSems[1] = {renderFinishedSemaphores[frame]};
    VkPipelineStageFlags waitStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSems;
    submitInfo.pWaitDstStageMask = &waitStage;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &swapchainCommandBuffers[frame];
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSems;

    if (vkQueueSubmit(queue, 1, &submitInfo, inFlightFences[frame]) != VK_SUCCESS) {
        printf("failed to submit rendering\n");
        exit(0);
    }

    VkPresentInfoKHR presentInfo = {};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSems;
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = &swapchain;
    presentInfo.pImageIndices = &index;

    vkQueuePresentKHR(queue, &presentInfo);
}

