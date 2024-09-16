#include <SDL2/SDL_vulkan.h>
#include "holder.h"
#include <stdio.h>
#include <stdlib.h>

void createInstance(VkInstance* pInstance) {
    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.apiVersion = VK_API_VERSION_1_0;

    uint32_t extCount;
    SDL_Vulkan_GetInstanceExtensions(window, &extCount, NULL);
    const char* exts[extCount];
    SDL_Vulkan_GetInstanceExtensions(window, &extCount, exts);

    const char* layers[1] = {"VK_LAYER_KHRONOS_validation"};
    #ifdef VALIDATION
    uint8_t layerCount = 1;
    #else
    uint8_t layerCount = 0;
    #endif

    VkInstanceCreateInfo instInfo = {};
    instInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instInfo.pApplicationInfo = &appInfo;
    instInfo.enabledExtensionCount = extCount;
    instInfo.ppEnabledExtensionNames = exts;
    instInfo.enabledLayerCount = layerCount;
    instInfo.ppEnabledLayerNames = layers;

    if (vkCreateInstance(&instInfo, NULL, pInstance) != VK_SUCCESS) {
        printf("failed to create vulkan instance\n");
        exit(0);
    }
}
