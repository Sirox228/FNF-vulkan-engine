#ifndef PHYSICAL_DEVICE_H
#define PHYSICAL_DEVICE_H

#include <vulkan/vulkan.h>

void findPhysicalDevice(VkPhysicalDevice* pPhysicalDevice, VkSurfaceFormatKHR* pSurfaceFormat, VkPresentModeKHR* pSurfacePresentMode, VkSurfaceCapabilitiesKHR* pSurfaceCapabilities);

#endif
