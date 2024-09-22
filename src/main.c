#include <SDL2/SDL.h>
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
#include <stdio.h>
#include <stdlib.h>

int main() {
    SDL_Init(SDL_INIT_EVENTS);
    window = SDL_CreateWindow("FNF", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_VULKAN | SDL_WINDOW_ALLOW_HIGHDPI);

    if (window == NULL) {
        printf("failed to create sdl window\n");
        exit(0);
    }

    loadVulkanLoaderFunctions();
    createInstance(&instance);
    loadVulkanInstanceFunctions(instance);
    createSurface(&surface);
    findPhysicalDevice(&physicalDevice, &surfaceFormat, &surfacePresentMode, &surfaceCapabilities);
    createDevice(&device, physicalDevice);
    loadVulkanDeviceFunctions(device);
    vkGetDeviceQueue(device, queueFamilyIndex, 0, &graphicsQueue);
    createSwapchain(&swapchain, surface);

    uint32_t swapchainImageCount;
    vkGetSwapchainImagesKHR(device, swapchain, &swapchainImageCount, NULL);
    VkImage swapchainImages[swapchainImageCount];
    vkGetSwapchainImagesKHR(device, swapchain, &swapchainImageCount, swapchainImages);

    VkImageView swapchainImageViews[swapchainImageCount];
    for (uint32_t i = 0; i < swapchainImageCount; i++)
        createImageView(swapchainImages[i], surfaceFormat.format, VK_IMAGE_VIEW_TYPE_2D, 0, 1, VK_IMAGE_ASPECT_COLOR_BIT, &swapchainImageViews[i]);

    createRenderPass(&renderPass);

    fillDefaultStates();

    {
        VkDescriptorSetLayoutBinding spriteImageBinding = {};
        createDescriptorSetLayoutBinding(0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_FRAGMENT_BIT, &spriteImageBinding);
        createDescriptorSetLayout(&spriteImageBinding, 1, &staticSpriteDescriptorSetLayout);

        VkDescriptorSetLayoutBinding spriteBufferBinding = {};
        createDescriptorSetLayoutBinding(1, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT, &spriteBufferBinding);
        createDescriptorSetLayout((VkDescriptorSetLayoutBinding[]){spriteBufferBinding, spriteImageBinding}, 2, &animatedSpriteDescriptorSetLayout);
    }

    createPipelineLayout(&staticSpritePipelineLayout, &staticSpriteDescriptorSetLayout, 1, NULL, 0);
    {
        VkPushConstantRange pcr = {};
        pcr.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
        pcr.offset = 0;
        pcr.size = sizeof(uint32_t);
        createPipelineLayout(&animatedSpritePipelineLayout, &animatedSpriteDescriptorSetLayout, 1, &pcr, 1);
    }

    {
        VkDynamicState dynamicStates[2] = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};

        VkPipelineDynamicStateCreateInfo dynamicStateInfo = {};
        dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dynamicStateInfo.dynamicStateCount = 2;
        dynamicStateInfo.pDynamicStates = dynamicStates;

        VkPipelineViewportStateCreateInfo viewportStateInfo = defaultViewportStateInfo;
        viewportStateInfo.pViewports = NULL;
        viewportStateInfo.pScissors = NULL;

        createGraphicsPipeline(&staticSpritePipeline, staticSpritePipelineLayout, renderPass, 0, "assets/shaders/quad/quad.vert.spv", "assets/shaders/quad/quad.frag.spv", NULL, NULL, &defaultVertexInputStateInfo, &defaultInputAssemblyStateInfo, NULL, &defaultDepthStencilStateInfo, &viewportStateInfo, &defaultRasterizationStateInfo, &defaultMultisampleStateInfo, &defaultBlendStateInfo, &dynamicStateInfo);
        createGraphicsPipeline(&animatedSpritePipeline, animatedSpritePipelineLayout, renderPass, 0, "assets/shaders/animated/animated.vert.spv", "assets/shaders/animated/animated.frag.spv", NULL, NULL, &defaultVertexInputStateInfo, &defaultInputAssemblyStateInfo, NULL, &defaultDepthStencilStateInfo, &viewportStateInfo, &defaultRasterizationStateInfo, &defaultMultisampleStateInfo, &defaultBlendStateInfo, &dynamicStateInfo);
    }

    createCommandPool(&mainCommandPool, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
    createCommandPool(&shortCommandPool, VK_COMMAND_POOL_CREATE_TRANSIENT_BIT);

    createSwapchainCommandBuffers(swapchainCommandBuffers, mainCommandPool);

    VkFramebuffer swapchainFramebuffers[swapchainImageCount];
    for (uint32_t i = 0; i < swapchainImageCount; i++)
        createFramebuffer((VkImageView[]){swapchainImageViews[i]}, 1, renderPass, swapchainExtent, &swapchainFramebuffers[i]);

    for (uint32_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        createSemaphore(&imageAvailableSemaphores[i]);
        createSemaphore(&renderFinishedSemaphores[i]);
        createFence(1, &inFlightFences[i]);
    }

    createAndFillBufferFromStaging(sizeof(vertex) * QUAD_VERT_NUM, quadVertexData, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &quadVertexBuffer, &quadVertexBufferMemory);
    createAndFillBufferFromStaging(sizeof(uint32_t) * QUAD_IDX_NUM, quadIndexData, VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &quadIndexBuffer, &quadIndexBufferMemory);

    sprite spr = staticSpriteCreate("assets/textures/bruh.png", 0.0f, 0.0f);
    spr.viewport.width = WINDOW_WIDTH;
    spr.viewport.height = WINDOW_HEIGHT;

    sprite spr2 = staticSpriteCreate("assets/textures/bruh.png", 0.0f, 0.0f);
    spr2.scaleX = 10.0;

    sprite spr3 = staticSpriteCreate("assets/textures/bruh.png", 0.0f, 0.0f);
    spr3.viewport.x = WINDOW_WIDTH - spr3.viewport.width * spr3.scaleX;
    spr3.scaleX = 2.0;
    spr3.scaleY = 5.0;

    sprite spr4 = staticSpriteCreate("assets/textures/crown.png", 0.0f, 0.0f);
    spr4.scaleX = 2.0f;
    spr4.scaleY = 2.0f;
    spr4.viewport.x = (WINDOW_WIDTH / 2.0f) - ((spr4.viewport.width * spr4.scaleX) / 2.0f);
    spr4.viewport.y -= (spr4.viewport.height * spr4.scaleY) / 2;

    sprite agoti = animatedSpriteCreate("assets/animatedSprites/AGOTI.png", "assets/animatedSprites/AGOTI.xml", 100, 100, 24.0f);
    agoti.scaleX = 0.75f;
    agoti.scaleY = 0.75f;

    spritePlayAnimation("Agoti_Idle", &agoti);

    sprite sprites[] = {spr, spr4, spr2, spr3, agoti};

    SDL_Event event;
    float startTime = SDL_GetTicks();
    while (active) {
        while (SDL_PollEvent(&event)) {
    		    if (event.type == SDL_QUIT) {
    			    active = 0;
    			    break;
    		    }
        }

        render(swapchainFramebuffers, sprites, 5);
        frame = (frame + 1) % MAX_FRAMES_IN_FLIGHT;
        float curTime = SDL_GetTicks();
        timeDelta = curTime - startTime;
        startTime = curTime;
        for (uint32_t i = 0; i < 5; i++) {
            if (sprites[i].isAnimated) {
                if (sprites[i].accumulator >= sprites[i].delay) {
                    sprites[i].accumulator = 0.0f;
                    sprites[i].animFrame++;
                    if (sprites[i].atlas.animations[sprites[i].animIndex].fcount <= sprites[i].animFrame) {
                        sprites[i].animFrame = 0;
                    }
                } else {
                    sprites[i].accumulator += timeDelta;
                }
            }
        }
    }

    vkDeviceWaitIdle(device);

    staticSpriteDestroy(&spr);
    staticSpriteDestroy(&spr2);
    staticSpriteDestroy(&spr3);
    staticSpriteDestroy(&spr4);
    animatedSpriteDestroy(&agoti);

    vkDestroyDescriptorSetLayout(device, staticSpriteDescriptorSetLayout, NULL);
    vkDestroyDescriptorSetLayout(device, animatedSpriteDescriptorSetLayout, NULL);
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
    vkDestroyCommandPool(device, mainCommandPool, NULL);
    vkDestroyPipeline(device, staticSpritePipeline, NULL);
    vkDestroyPipelineLayout(device, staticSpritePipelineLayout, NULL);
    vkDestroyPipeline(device, animatedSpritePipeline, NULL);
    vkDestroyPipelineLayout(device, animatedSpritePipelineLayout, NULL);
    vkDestroyRenderPass(device, renderPass, NULL);
    for (uint32_t i = 0; i < swapchainImageCount; i++)
        vkDestroyImageView(device, swapchainImageViews[i], NULL);
    vkDestroySwapchainKHR(device, swapchain, NULL);
    vkDestroyDevice(device, NULL);
    vkDestroySurfaceKHR(instance, surface, NULL);
    vkDestroyInstance(instance, NULL);

    SDL_DestroyWindow(window);
    SDL_Quit();
}
