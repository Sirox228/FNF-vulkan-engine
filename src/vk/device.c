#include "holder.h"
#include <stdio.h>
#include <stdlib.h>

#define DEVICE_EXTS_COUNT 1

void createDevice(VkDevice* pDevice, VkPhysicalDevice sourcePhysicalDevice) {
    const char* layers[1] = {"VK_LAYER_KHRONOS_validation"};
    #ifdef VALIDATION
    uint8_t layerCount = 1;
    #else
    uint8_t layerCount = 0;
    #endif
    const char* deviceExts[DEVICE_EXTS_COUNT] = {"VK_KHR_swapchain"};

    float queuePriority = 1.0f;
    VkDeviceQueueCreateInfo queueInfo = {};
    queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueInfo.queueFamilyIndex = queueFamilyIndex;
    queueInfo.queueCount = 1;
    queueInfo.pQueuePriorities = &queuePriority;

    VkDeviceCreateInfo deviceInfo = {};
    deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceInfo.pQueueCreateInfos = &queueInfo;
    deviceInfo.queueCreateInfoCount = 1;
    deviceInfo.pEnabledFeatures = &deviceFeatures;
    deviceInfo.enabledExtensionCount = DEVICE_EXTS_COUNT;
    deviceInfo.ppEnabledExtensionNames = deviceExts;
    deviceInfo.enabledLayerCount = layerCount;
    deviceInfo.ppEnabledLayerNames = layers;

    if (vkCreateDevice(sourcePhysicalDevice, &deviceInfo, NULL, pDevice) != VK_SUCCESS) {
    	printf("failed to create logical device\n");
        exit(0);
    }
}
