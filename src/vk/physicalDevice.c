#include "holder.h"
#include <stdio.h>
#include <stdlib.h>

#define DEVICE_EXTS_COUNT 1

void findPhysicalDevice(VkPhysicalDevice* pPhysicalDevice, VkSurfaceFormatKHR* pSurfaceFormat, VkPresentModeKHR* pSurfacePresentMode, VkSurfaceCapabilitiesKHR* pSurfaceCapabilities) {
    const char* deviceExts[DEVICE_EXTS_COUNT] = {"VK_KHR_swapchain"};

    uint32_t deviceCount;
    vkEnumeratePhysicalDevices(instance, &deviceCount, NULL);
    VkPhysicalDevice devices[deviceCount];
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices);

    for (int i = 0; i < deviceCount; i++) {
        uint32_t extCount;
        vkEnumerateDeviceExtensionProperties(devices[i], NULL, &extCount, NULL);
        VkExtensionProperties exts[extCount];
        vkEnumerateDeviceExtensionProperties(devices[i], NULL, &extCount, exts);

        uint8_t supportsExts = 1;
        for (int j = 0; j < DEVICE_EXTS_COUNT; j++) {
            uint8_t found = 0;

            for (int k = 0; k < extCount; k++) {
                if (!strcmp(deviceExts[j], exts[k].extensionName)) {
                    found = 1;
                    break;
                }
            }

            if (!found) {
                supportsExts = 0;
                break;
            }
        }

        uint32_t queueFamilyCount;
    	vkGetPhysicalDeviceQueueFamilyProperties(devices[i], &queueFamilyCount, NULL);
    	VkQueueFamilyProperties queueFamilyProps[queueFamilyCount];
        vkGetPhysicalDeviceQueueFamilyProperties(devices[i], &queueFamilyCount, queueFamilyProps);

        uint8_t foundQueue = 0;
        for (uint32_t j = 0; j < queueFamilyCount; j++) {
    		if (queueFamilyProps[j].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
    			VkBool32 presentSupported = 0;
    			vkGetPhysicalDeviceSurfaceSupportKHR(devices[i], j, surface, &presentSupported);
    			
    			if (presentSupported) {
    				queueFamilyIndex = j;
                    foundQueue = 1;
    				break;
    			}
    		}
    	}

        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(devices[i], surface, pSurfaceCapabilities);

    	uint32_t formatCount;
    	vkGetPhysicalDeviceSurfaceFormatsKHR(devices[i], surface, &formatCount, NULL);
    	VkSurfaceFormatKHR surfaceFormats[formatCount];
    	vkGetPhysicalDeviceSurfaceFormatsKHR(devices[i], surface, &formatCount, surfaceFormats);

        uint8_t foundFormat = 0;
        for (int j = 0; j < formatCount; j++) {
            if (surfaceFormats[j].format == VK_FORMAT_R8G8B8A8_SRGB && surfaceFormats[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
    			*pSurfaceFormat = surfaceFormats[j];
                foundFormat = 1;
    		}
    	}
    	if (!foundFormat && formatCount > 0) {
            *pSurfaceFormat = surfaceFormats[0];
        }

    	uint32_t presentModeCount;
    	vkGetPhysicalDeviceSurfacePresentModesKHR(devices[i], surface, &presentModeCount, NULL);
    	VkPresentModeKHR surfacePresentModes[presentModeCount];
    	vkGetPhysicalDeviceSurfacePresentModesKHR(devices[i], surface, &presentModeCount, surfacePresentModes);

        uint8_t foundPresentMode = 0;
        for (int j = 0; j < presentModeCount; j++) {
            if (surfacePresentModes[j] == VK_PRESENT_MODE_MAILBOX_KHR) {
    			*pSurfacePresentMode = VK_PRESENT_MODE_MAILBOX_KHR;
                foundPresentMode = 1;
    		}
    	}
    	if (!foundPresentMode) {
            *pSurfacePresentMode = VK_PRESENT_MODE_FIFO_KHR;
        }

        vkGetPhysicalDeviceMemoryProperties(devices[i], &physicalDeviceMemProps);
        vkGetPhysicalDeviceProperties(devices[i], &deviceProperties);
        vkGetPhysicalDeviceFeatures(devices[i], &deviceFeatures);

        if (supportsExts && foundQueue && formatCount > 0 && presentModeCount > 0) {
            *pPhysicalDevice = devices[i];
            break;
        }
    }
    
    if (*pPhysicalDevice == VK_NULL_HANDLE) {
        printf("failed to find suitable vulkan physical device\n");
        exit(0);
    }
}
