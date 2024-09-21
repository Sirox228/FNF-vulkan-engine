#ifndef HOLDER_H
#define HOLDER_H

#include <SDL2/SDL.h>
#include <vulkan/vulkan.h>
#include "vk/vertex.h"
#include "config.h"

extern SDL_Window* window;
extern uint8_t active;
extern VkInstance instance;
extern VkSurfaceKHR surface;
extern VkPhysicalDevice physicalDevice;
extern VkSurfaceCapabilitiesKHR surfaceCapabilities;
extern VkSurfaceFormatKHR surfaceFormat;
extern VkPresentModeKHR surfacePresentMode;
extern uint32_t queueFamilyIndex;
extern VkPhysicalDeviceMemoryProperties physicalDeviceMemProps;
extern VkPhysicalDeviceProperties deviceProperties;
extern VkPhysicalDeviceFeatures deviceFeatures;
extern VkDevice device;
extern VkQueue graphicsQueue;
extern VkExtent2D swapchainExtent;
extern VkSwapchainKHR swapchain;
extern VkRenderPass renderPass;
extern VkPipelineLayout staticSpritePipelineLayout;
extern VkPipeline staticSpritePipeline;
extern VkCommandPool mainCommandPool;
extern VkCommandPool shortCommandPool;
extern VkCommandBuffer swapchainCommandBuffers[MAX_FRAMES_IN_FLIGHT];

extern VkBuffer quadVertexBuffer;
extern VkDeviceMemory quadVertexBufferMemory;
extern VkBuffer quadIndexBuffer;
extern VkDeviceMemory quadIndexBufferMemory;

extern VkSemaphore imageAvailableSemaphores[MAX_FRAMES_IN_FLIGHT];
extern VkSemaphore renderFinishedSemaphores[MAX_FRAMES_IN_FLIGHT];
extern VkFence inFlightFences[MAX_FRAMES_IN_FLIGHT];
extern uint32_t frame;


#define QUAD_VERT_NUM 4
extern vertex quadVertexData[QUAD_VERT_NUM];
#define QUAD_IDX_NUM 6
extern uint32_t quadIndexData[QUAD_IDX_NUM];

// default pipeline states
extern VkPipelineShaderStageCreateInfo defaultCompStageInfo;
extern VkPipelineShaderStageCreateInfo defaultFragStageInfo;
extern VkPipelineShaderStageCreateInfo defaultVertStageInfo;
extern VkPipelineShaderStageCreateInfo defaultTescStageInfo;
extern VkPipelineShaderStageCreateInfo defaultTeseStageInfo;
extern VkVertexInputBindingDescription defaultVertInpBindDesc;
extern VkVertexInputAttributeDescription defaultVertInpAttrDesc;
extern VkPipelineVertexInputStateCreateInfo defaultVertexInputStateInfo;
extern VkPipelineInputAssemblyStateCreateInfo defaultInputAssemblyStateInfo;
// (sirox) if you're using tessellation shaders, specify this input assembly state
extern VkPipelineInputAssemblyStateCreateInfo defaultTessellationInputAssemblyStateInfo;
extern VkPipelineTessellationStateCreateInfo defaultTessellationStateInfo;
extern VkPipelineDepthStencilStateCreateInfo defaultDepthStencilStateInfo;
extern VkViewport defaultViewport;
extern VkRect2D defaultScissor;
extern VkPipelineViewportStateCreateInfo defaultViewportStateInfo;
extern VkPipelineRasterizationStateCreateInfo defaultRasterizationStateInfo;
extern VkPipelineMultisampleStateCreateInfo defaultMultisampleStateInfo;
extern VkPipelineColorBlendAttachmentState defaultAlphaBlendAttachment;
extern VkPipelineColorBlendStateCreateInfo defaultBlendStateInfo;

extern VkClearValue clearColorValue;

extern VkDescriptorSetLayout staticSpriteDescriptorSetLayout;
extern VkDescriptorSetLayout animatedSpriteDescriptorSetLayout;
extern VkPipelineLayout animatedSpritePipelineLayout;
extern VkPipeline animatedSpritePipeline;

extern float timeDelta;

#endif
