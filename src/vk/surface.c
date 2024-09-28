#include <SDL_vulkan.h>
#include "holder.h"
#include <stdio.h>

void createSurface(VkSurfaceKHR* pSurface) {
    if (SDL_Vulkan_CreateSurface(window, instance, pSurface) != SDL_TRUE) {
        printf("failed to create surface");
        exit(0);
    }
}
