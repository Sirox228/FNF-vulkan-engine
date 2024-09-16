#include <SDL2/SDL_vulkan.h>
#include "holder.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

void createSwapchain(VkSwapchainKHR* pSwapchain, VkSurfaceKHR sourceSurface) {
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

    uint32_t imageCount = surfaceCapabilities.minImageCount + 1;
    if (surfaceCapabilities.maxImageCount > 0 && imageCount > surfaceCapabilities.maxImageCount)
    	imageCount = surfaceCapabilities.maxImageCount;

    VkSwapchainCreateInfoKHR swapchainInfo = {};
    swapchainInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapchainInfo.surface = sourceSurface;
    swapchainInfo.minImageCount = imageCount;
    swapchainInfo.imageFormat = surfaceFormat.format;
    swapchainInfo.imageColorSpace = surfaceFormat.colorSpace;
    swapchainInfo.imageExtent = swapchainExtent;
    swapchainInfo.imageArrayLayers = 1;
    swapchainInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    swapchainInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    swapchainInfo.preTransform = surfaceCapabilities.currentTransform;
    swapchainInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    swapchainInfo.presentMode = surfacePresentMode;
    swapchainInfo.clipped = VK_TRUE;
    swapchainInfo.oldSwapchain = VK_NULL_HANDLE;

    if (vkCreateSwapchainKHR(device, &swapchainInfo, NULL, pSwapchain) != VK_SUCCESS) {
        printf("failed to create swapchain\n");
        exit(0);
    }
}
