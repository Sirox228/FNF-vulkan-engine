#include <SDL2/SDL.h>
#include <vulkan/vulkan.h>
#include "vk/vertex.h"
#include "config.h"

SDL_Window* window;
uint8_t active = 1;
VkInstance instance;
VkSurfaceKHR surface;
VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
VkSurfaceCapabilitiesKHR surfaceCapabilities;
VkSurfaceFormatKHR surfaceFormat;
VkPresentModeKHR surfacePresentMode;
uint32_t queueFamilyIndex;
VkPhysicalDeviceMemoryProperties physicalDeviceMemProps;
VkPhysicalDeviceProperties deviceProperties;
VkPhysicalDeviceFeatures deviceFeatures;
VkDevice device;
VkQueue graphicsQueue;
VkExtent2D swapchainExtent;
VkSwapchainKHR swapchain;
VkRenderPass renderPass;
VkPipelineLayout staticSpritePipelineLayout;
VkPipeline staticSpritePipeline;
VkCommandPool mainCommandPool;
VkCommandPool shortCommandPool;
VkCommandBuffer swapchainCommandBuffers[MAX_FRAMES_IN_FLIGHT];

VkBuffer quadVertexBuffer;
VkDeviceMemory quadVertexBufferMemory;
VkBuffer quadIndexBuffer;
VkDeviceMemory quadIndexBufferMemory;

VkSemaphore imageAvailableSemaphores[MAX_FRAMES_IN_FLIGHT];
VkSemaphore renderFinishedSemaphores[MAX_FRAMES_IN_FLIGHT];
VkFence inFlightFences[MAX_FRAMES_IN_FLIGHT];
uint32_t frame = 0;


#define QUAD_VERT_NUM 4
vertex quadVertexData[QUAD_VERT_NUM] = {
    {{-1.0f, -1.0f}},
    {{1.0f, -1.0f}},
    {{-1.0f, 1.0f}},
    {{1.0f, 1.0f}}
};

#define QUAD_IDX_NUM 6
uint32_t quadIndexData[QUAD_IDX_NUM] = {
    0, 1, 2,
    2, 1, 3
};


// default pipeline states
VkPipelineShaderStageCreateInfo defaultCompStageInfo = {};
VkPipelineShaderStageCreateInfo defaultFragStageInfo = {};
VkPipelineShaderStageCreateInfo defaultVertStageInfo = {};
VkPipelineShaderStageCreateInfo defaultTescStageInfo = {};
VkPipelineShaderStageCreateInfo defaultTeseStageInfo = {};
VkVertexInputBindingDescription defaultVertInpBindDesc = {};
VkVertexInputAttributeDescription defaultVertInpAttrDesc = {};
VkPipelineVertexInputStateCreateInfo defaultVertexInputStateInfo = {};
VkPipelineInputAssemblyStateCreateInfo defaultInputAssemblyStateInfo = {};
// (sirox) if you're using tessellation shaders, specify this input assembly state
VkPipelineInputAssemblyStateCreateInfo defaultTessellationInputAssemblyStateInfo = {};
VkPipelineTessellationStateCreateInfo defaultTessellationStateInfo = {};
VkPipelineDepthStencilStateCreateInfo defaultDepthStencilStateInfo = {};
VkViewport defaultViewport = {};
VkRect2D defaultScissor = {};
VkPipelineViewportStateCreateInfo defaultViewportStateInfo = {};
VkPipelineRasterizationStateCreateInfo defaultRasterizationStateInfo = {};
VkPipelineMultisampleStateCreateInfo defaultMultisampleStateInfo = {};
VkPipelineColorBlendAttachmentState defaultAlphaBlendAttachment = {};
VkPipelineColorBlendStateCreateInfo defaultBlendStateInfo = {};

VkClearValue clearColorValue = {.color = (VkClearColorValue){0.0f, 0.0f, 0.0f, 1.0f}};

VkDescriptorSetLayout staticSpriteDescriptorSetLayout;
VkDescriptorSetLayout animatedSpriteDescriptorSetLayout;
VkPipelineLayout animatedSpritePipelineLayout;
VkPipeline animatedSpritePipeline;

float timeDelta = 0.0f;
