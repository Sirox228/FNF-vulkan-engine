#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>
#include "ma/audio.h"
#include "holder.h"
#include "config.h"
#include "vk/instance.h"
#include "vk/surface.h"
#include "vk/physicalDevice.h"
#include "vk/device.h"
#include "vk/swapchain.h"
#include "vk/renderPass.h"
#include "vk/pipeline.h"
#include "vk/commandPool.h"
#include "vk/commandBuffer.h"
#include "vk/framebuffer.h"
#include "vk/sync.h"
#include "render.h"
#include "vk/vkutil.h"
#include "vk/descriptor.h"
#include "vk/image.h"
#include "vk/imageView.h"
#include "vk/buffer.h"
#include "vk/sampler.h"
#include "util.h"
#include "states/titlestate.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    SDL_Init(SDL_INIT_EVENTS);
    window = SDL_CreateWindow("Friday Night Funkin'", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_VULKAN | SDL_WINDOW_ALLOW_HIGHDPI);

    if (window == NULL) {
        printf("failed to create sdl window\n");
        exit(0);
    }

    // miniaudio initialization
    //
    initAudioDevice(&maDevice, &maDecoder);
    ma_decoder_init_file("assets/music/freakyMenu/freakyMenu.flac", NULL, &maDecoder);

    // vulkan global data and dynamic functions loading (if VK_NO_PROTOTYPES is enabled)
    //
    loadVulkanLoaderFunctions();
    createInstance(&instance);
    createSurface(&surface);
    loadVulkanInstanceFunctions(instance);
    findPhysicalDevice(&physicalDevice);
    createDevice(physicalDevice, &device);
    loadVulkanDeviceFunctions(device);
    vkGetDeviceQueue(device, queueFamilyIndex, 0, &queue);

    // render passes
    //
    {
        VkAttachmentDescription prePostProcessColorAttachmentDescription = {}, postProcessColorAttachmentDescription = {};
        createAttachmentDescription(surfaceFormat.format, VK_SAMPLE_COUNT_1_BIT, VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_STORE, VK_ATTACHMENT_LOAD_OP_DONT_CARE, VK_ATTACHMENT_STORE_OP_DONT_CARE, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, &prePostProcessColorAttachmentDescription);
        createAttachmentDescription(surfaceFormat.format, VK_SAMPLE_COUNT_1_BIT, VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_STORE, VK_ATTACHMENT_LOAD_OP_DONT_CARE, VK_ATTACHMENT_STORE_OP_DONT_CARE, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, &postProcessColorAttachmentDescription);

        VkAttachmentReference baseColorAttachmentReference = {};
        createAttachmentReference(0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, &baseColorAttachmentReference);

        VkSubpassDescription baseSubpassDescription = {};
        createSubpassDescription(VK_PIPELINE_BIND_POINT_GRAPHICS, 1, &baseColorAttachmentReference, NULL, NULL, 0, NULL, 0, NULL, &baseSubpassDescription);

        VkSubpassDependency prePostProcessSubpassDependencyPrePass = {}, prePostProcessSubpassDependencyPostPass = {};
        createSubpassDependency(VK_SUBPASS_EXTERNAL, 0, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_ACCESS_MEMORY_READ_BIT, VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT, VK_DEPENDENCY_BY_REGION_BIT, &prePostProcessSubpassDependencyPrePass);
        createSubpassDependency(0, VK_SUBPASS_EXTERNAL, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT, VK_ACCESS_MEMORY_READ_BIT, VK_DEPENDENCY_BY_REGION_BIT, &prePostProcessSubpassDependencyPostPass);

        createRenderPass(1, &prePostProcessColorAttachmentDescription, 1, &baseSubpassDescription, 2, (VkSubpassDependency[]){prePostProcessSubpassDependencyPrePass, prePostProcessSubpassDependencyPostPass}, &baseRenderPass);
        createRenderPass(1, &postProcessColorAttachmentDescription, 1, &baseSubpassDescription, 0, NULL, &finalRenderPass);
    }

    if (surfaceCapabilities.currentExtent.width != UINT32_MAX) {
    	swapchainExtent = surfaceCapabilities.currentExtent;
    } else {
    	int32_t w, h;
    	SDL_Vulkan_GetDrawableSize(window, &w, &h);

    	swapchainExtent = (VkExtent2D){
    		clamp((uint32_t)w, surfaceCapabilities.minImageExtent.width, surfaceCapabilities.maxImageExtent.width),
    		clamp((uint32_t)h, surfaceCapabilities.minImageExtent.height, surfaceCapabilities.maxImageExtent.height)
    	};
    }

    // image data
    //
    createImage(WINDOW_WIDTH, WINDOW_HEIGHT, surfaceFormat.format, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, VK_IMAGE_LAYOUT_UNDEFINED, 0, 1, &prePostProcessImage, &prePostProcessImageMemory);
    createImageView(prePostProcessImage, surfaceFormat.format, VK_IMAGE_VIEW_TYPE_2D, 0, 1, VK_IMAGE_ASPECT_COLOR_BIT, &prePostProcessImageView);
    createFramebuffer(&prePostProcessImageView, 1, baseRenderPass, swapchainExtent, &prePostProcessImageFramebuffer);
    createSampler(VK_FILTER_LINEAR, VK_FILTER_LINEAR, &prePostProcessImageSampler);

    createImage(WINDOW_WIDTH, WINDOW_HEIGHT, surfaceFormat.format, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, VK_IMAGE_LAYOUT_UNDEFINED, 0, 1, &hdrBloomDownsampledImage, &hdrBloomDownsampledImageMemory);
    createImageView(hdrBloomDownsampledImage, surfaceFormat.format, VK_IMAGE_VIEW_TYPE_2D, 0, 1, VK_IMAGE_ASPECT_COLOR_BIT, &hdrBloomDownsampledImageView);
    createFramebuffer(&hdrBloomDownsampledImageView, 1, baseRenderPass, swapchainExtent, &hdrBloomDownsampledImageFramebuffer);
    createSampler(VK_FILTER_LINEAR, VK_FILTER_LINEAR, &hdrBloomDownsampledImageSampler);

    // swapchain data
    //
    createSwapchain(surface, &swapchain);
    createCommandPool(VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT, &swapchainCommandPool);
    createCommandBuffers(MAX_FRAMES_IN_FLIGHT, swapchainCommandPool, swapchainCommandBuffers);

    uint32_t swapchainImageCount;
    vkGetSwapchainImagesKHR(device, swapchain, &swapchainImageCount, NULL);
    VkImage swapchainImages[swapchainImageCount];
    vkGetSwapchainImagesKHR(device, swapchain, &swapchainImageCount, swapchainImages);

    VkImageView swapchainImageViews[swapchainImageCount];
    for (uint32_t i = 0; i < swapchainImageCount; i++)
        createImageView(swapchainImages[i], surfaceFormat.format, VK_IMAGE_VIEW_TYPE_2D, 0, 1, VK_IMAGE_ASPECT_COLOR_BIT, &swapchainImageViews[i]);

    VkFramebuffer swapchainFramebuffers[swapchainImageCount];
    for (uint32_t i = 0; i < swapchainImageCount; i++)
        createFramebuffer((VkImageView[]){swapchainImageViews[i]}, 1, finalRenderPass, swapchainExtent, &swapchainFramebuffers[i]);

    for (uint32_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        createSemaphore(&imageAvailableSemaphores[i]);
        createSemaphore(&renderFinishedSemaphores[i]);
        createFence(1, &inFlightFences[i]);
    }

    // utility data
    //
    createCommandPool(VK_COMMAND_POOL_CREATE_TRANSIENT_BIT, &shortCommandPool);

    // fill default pipeline states
    fillDefaultStates();

    // descriptor set layouts
    //
    {
        VkDescriptorSetLayoutBinding imageBinding = {}, hdrBloomDownsampledImageBinding = {};
        createDescriptorSetLayoutBinding(0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_FRAGMENT_BIT, &imageBinding);
        createDescriptorSetLayoutBinding(1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_FRAGMENT_BIT, &hdrBloomDownsampledImageBinding);
        createDescriptorSetLayout(&imageBinding, 1, &staticSpriteDescriptorSetLayout);
        createDescriptorSetLayout(&imageBinding, 1, &prePostProcessImageDescriptorSetLayout);
        createDescriptorSetLayout(&hdrBloomDownsampledImageBinding, 1, &hdrBloomDownsampledImageDescriptorSetLayout);

        VkDescriptorSetLayoutBinding spriteBufferBinding = {};
        createDescriptorSetLayoutBinding(1, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT, &spriteBufferBinding);
        createDescriptorSetLayout((VkDescriptorSetLayoutBinding[]){spriteBufferBinding, imageBinding}, 2, &animatedSpriteDescriptorSetLayout);
    }

    // pipeline layouts
    //
    createPipelineLayout(&staticSpriteDescriptorSetLayout, 1, NULL, 0, &staticSpritePipelineLayout);
    createPipelineLayout(&prePostProcessImageDescriptorSetLayout, 1, NULL, 0, &hdrBloomDownsamplePipelineLayout);
    createPipelineLayout((VkDescriptorSetLayout[]){prePostProcessImageDescriptorSetLayout, hdrBloomDownsampledImageDescriptorSetLayout}, 2, NULL, 0, &hdrBloomUpsamplePipelineLayout);
    {
        VkPushConstantRange pcr = {};
        pcr.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
        pcr.offset = 0;
        pcr.size = sizeof(uint32_t);
        createPipelineLayout(&animatedSpriteDescriptorSetLayout, 1, &pcr, 1, &animatedSpritePipelineLayout);
    }

    // descriptor pools
    //
    {
        VkDescriptorPoolSize imagePoolSize = {};
        createDescriptorPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 2, &imagePoolSize);

        createDescriptorPool(2, &imagePoolSize, 1, &postProcessDescriptorPool);
    }

    // descriptor sets
    //
    {
        createDescriptorSets(postProcessDescriptorPool, 1, &prePostProcessImageDescriptorSetLayout, &prePostProcessImageDescriptorSet);
        createDescriptorSets(postProcessDescriptorPool, 1, &hdrBloomDownsampledImageDescriptorSetLayout, &hdrBloomDownsampledDescriptorSet);

        VkDescriptorImageInfo imageInfo = {}, downsampledImageInfo = {};
        createDescriptorImageInfo(prePostProcessImageSampler, prePostProcessImageView, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, &imageInfo);
        createDescriptorImageInfo(hdrBloomDownsampledImageSampler, hdrBloomDownsampledImageView, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, &downsampledImageInfo);

        VkWriteDescriptorSet prePostProcessWrite = {}, hdrBloomUpsampleWrite = {};
        createWriteDescriptorSet(prePostProcessImageDescriptorSet, 0, 1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, &imageInfo, NULL, &prePostProcessWrite);
        createWriteDescriptorSet(hdrBloomDownsampledDescriptorSet, 1, 1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, &downsampledImageInfo, NULL, &hdrBloomUpsampleWrite);

        vkUpdateDescriptorSets(device, 2, (VkWriteDescriptorSet[]){prePostProcessWrite, hdrBloomUpsampleWrite}, 0, NULL);
    }

    // pipelines
    //
    {
        VkDynamicState dynamicStates[2] = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};

        VkPipelineDynamicStateCreateInfo dynamicStateInfo = {};
        dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dynamicStateInfo.dynamicStateCount = 2;
        dynamicStateInfo.pDynamicStates = dynamicStates;

        VkPipelineViewportStateCreateInfo viewportStateInfo = defaultViewportStateInfo;
        viewportStateInfo.pViewports = NULL;
        viewportStateInfo.pScissors = NULL;

        VkPipelineColorBlendAttachmentState blendAttach = {};
        blendAttach.blendEnable = VK_FALSE;
        blendAttach.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

        VkPipelineColorBlendStateCreateInfo blendStateInfo = {};
        blendStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        blendStateInfo.logicOpEnable = VK_FALSE;
        blendStateInfo.attachmentCount = 1;
        blendStateInfo.pAttachments = &blendAttach;

        createGraphicsPipeline(staticSpritePipelineLayout, baseRenderPass, 0, "assets/shaders/quad/quad.vert.spv", "assets/shaders/quad/quad.frag.spv", NULL, NULL, &defaultVertexInputStateInfo, &defaultInputAssemblyStateInfo, NULL, &defaultDepthStencilStateInfo, &viewportStateInfo, &defaultRasterizationStateInfo, &defaultMultisampleStateInfo, &defaultBlendStateInfo, &dynamicStateInfo, &staticSpritePipeline);
        createGraphicsPipeline(animatedSpritePipelineLayout, baseRenderPass, 0, "assets/shaders/animated/animated.vert.spv", "assets/shaders/animated/animated.frag.spv", NULL, NULL, &defaultVertexInputStateInfo, &defaultInputAssemblyStateInfo, NULL, &defaultDepthStencilStateInfo, &viewportStateInfo, &defaultRasterizationStateInfo, &defaultMultisampleStateInfo, &defaultBlendStateInfo, &dynamicStateInfo, &animatedSpritePipeline);
        createGraphicsPipeline(hdrBloomDownsamplePipelineLayout, baseRenderPass, 0, "assets/shaders/hdrBloom/downsample.vert.spv", "assets/shaders/hdrBloom/downsample.frag.spv", NULL, NULL, &defaultVertexInputStateInfo, &defaultInputAssemblyStateInfo, NULL, &defaultDepthStencilStateInfo, &defaultViewportStateInfo, &defaultRasterizationStateInfo, &defaultMultisampleStateInfo, &blendStateInfo, NULL, &hdrBloomDownsamplePipeline);
        createGraphicsPipeline(hdrBloomUpsamplePipelineLayout, finalRenderPass, 0, "assets/shaders/hdrBloom/upsample.vert.spv", "assets/shaders/hdrBloom/upsample.frag.spv", NULL, NULL, &defaultVertexInputStateInfo, &defaultInputAssemblyStateInfo, NULL, &defaultDepthStencilStateInfo, &defaultViewportStateInfo, &defaultRasterizationStateInfo, &defaultMultisampleStateInfo, &blendStateInfo, NULL, &hdrBloomUpsamplePipeline);
    }

    // buffers
    //
    createAndFillBufferFromStaging(sizeof(vertex) * QUAD_VERT_NUM, quadVertexData, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &quadVertexBuffer, &quadVertexBufferMemory);
    createAndFillBufferFromStaging(sizeof(uint32_t) * QUAD_IDX_NUM, quadIndexData, VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &quadIndexBuffer, &quadIndexBufferMemory);
    
    stateCreate = titlestateCreate;
    stateUpdate = titlestateUpdate;
    stateDestroy = titlestateDestroy;

    stateCreate();

    SDL_Event event;
    double startTime = SDL_GetPerformanceCounter();
    while (active) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                active = 0;
                break;
            }
        }

        render(swapchainFramebuffers, sprites, globalSpriteCount);
        frame = (frame + 1) % MAX_FRAMES_IN_FLIGHT;
        double curTime = SDL_GetPerformanceCounter();
        timeDelta = (curTime - startTime) / (double)SDL_GetPerformanceFrequency();
        //printf("%f\n", timeDelta);
        startTime = curTime;
        for (uint32_t i = 0; i < globalSpriteCount; i++) {
            if (sprites[i].isAnimated) {
                if (sprites[i].accumulator >= sprites[i].delay) {
                    while (sprites[i].accumulator >= sprites[i].delay) {
                        sprites[i].accumulator = sprites[i].accumulator - sprites[i].delay;
                        sprites[i].animFrame++;
                    }
                    if (sprites[i].atlas.animations[sprites[i].animIndex].fcount <= sprites[i].animFrame + 1) {
                        if (sprites[i].loopAnimation) sprites[i].animFrame = 0;
                        else sprites[i].animFrame = sprites[i].atlas.animations[sprites[i].animIndex].fcount - 1;
                    }
                }
                sprites[i].accumulator += timeDelta;
            }
        }

        stateUpdate();
    }

    stateDestroy();

    vkDeviceWaitIdle(device);

    vkDestroyDescriptorPool(device, postProcessDescriptorPool, NULL);
    vkDestroySampler(device, hdrBloomDownsampledImageSampler, NULL);
    vkDestroyFramebuffer(device, hdrBloomDownsampledImageFramebuffer, NULL);
    vkDestroyImageView(device, hdrBloomDownsampledImageView, NULL);
    vkDestroyImage(device, hdrBloomDownsampledImage, NULL);
    vkFreeMemory(device, hdrBloomDownsampledImageMemory, NULL);
    vkDestroySampler(device, prePostProcessImageSampler, NULL);
    vkDestroyFramebuffer(device, prePostProcessImageFramebuffer, NULL);
    vkDestroyImageView(device, prePostProcessImageView, NULL);
    vkDestroyImage(device, prePostProcessImage, NULL);
    vkFreeMemory(device, prePostProcessImageMemory, NULL);
    vkDestroyDescriptorSetLayout(device, staticSpriteDescriptorSetLayout, NULL);
    vkDestroyDescriptorSetLayout(device, animatedSpriteDescriptorSetLayout, NULL);
    vkDestroyDescriptorSetLayout(device, hdrBloomDownsampledImageDescriptorSetLayout, NULL);
    vkDestroyDescriptorSetLayout(device, prePostProcessImageDescriptorSetLayout, NULL);
    for (uint32_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        vkDestroyFence(device, inFlightFences[i], NULL);
        vkDestroySemaphore(device, renderFinishedSemaphores[i], NULL);
        vkDestroySemaphore(device, imageAvailableSemaphores[i], NULL);
    }
    for (uint32_t i = 0; i < swapchainImageCount; i++)
        vkDestroyFramebuffer(device, swapchainFramebuffers[i], NULL);
    vkDestroyBuffer(device, quadIndexBuffer, NULL);
    vkFreeMemory(device, quadIndexBufferMemory, NULL);
    vkDestroyBuffer(device, quadVertexBuffer, NULL);
    vkFreeMemory(device, quadVertexBufferMemory, NULL);
    vkDestroyCommandPool(device, shortCommandPool, NULL);
    vkDestroyCommandPool(device, swapchainCommandPool, NULL);
    vkDestroyPipeline(device, staticSpritePipeline, NULL);
    vkDestroyPipelineLayout(device, staticSpritePipelineLayout, NULL);
    vkDestroyPipeline(device, animatedSpritePipeline, NULL);
    vkDestroyPipelineLayout(device, animatedSpritePipelineLayout, NULL);
    vkDestroyPipeline(device, hdrBloomUpsamplePipeline, NULL);
    vkDestroyPipelineLayout(device, hdrBloomUpsamplePipelineLayout, NULL);
    vkDestroyPipeline(device, hdrBloomDownsamplePipeline, NULL);
    vkDestroyPipelineLayout(device, hdrBloomDownsamplePipelineLayout, NULL);
    vkDestroyRenderPass(device, baseRenderPass, NULL);
    vkDestroyRenderPass(device, finalRenderPass, NULL);
    for (uint32_t i = 0; i < swapchainImageCount; i++)
        vkDestroyImageView(device, swapchainImageViews[i], NULL);
    vkDestroySwapchainKHR(device, swapchain, NULL);
    vkDestroyDevice(device, NULL);
    vkDestroySurfaceKHR(instance, surface, NULL);
    vkDestroyInstance(instance, NULL);

    SDL_DestroyWindow(window);
    SDL_Quit();

    ma_decoder_uninit(&maDecoder);
    ma_device_uninit(&maDevice);
}
