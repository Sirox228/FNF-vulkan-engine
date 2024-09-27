#include <SDL2/SDL.h>
#include <ma/miniaudio.h>
#include <vulkan/vulkan.h>
#include "vk/vertex.h"
#include "config.h"
#include "sprite.h"

// game global variables
//
// flag which closes the game if set to 0
uint8_t active = 1;
// time passed since the last frame (time delta)
float timeDelta = 0.0f;

sprite* sprites;
uint32_t globalSpriteCount = 0;
void (*stateCreate)();
void (*stateEvent)(SDL_Event* event);
void (*stateUpdate)();
void (*stateDestroy)();

// conductor data
// bpm
float bpm = DEFAULT_BPM;
float startingBPM = DEFAULT_BPM;
float bpmOverride;
// song positions
float songPosition;
uint32_t currentBeat;
uint32_t currentStep;
float currentBeatTime;
float currentStepTime;
// duration
float beatLengthMs;
float stepLengthMs;

// sdl window handle
//
SDL_Window* window;

// miniaudio global data
//
ma_decoder maDecoder;
ma_device maDevice;
// device properties
ma_format outputFormat = ma_format_f32;
ma_uint32 outputChannels = 2;
ma_uint32 outputSampleRate = 44100;

// vulkan global data
//
VkInstance instance;
VkSurfaceKHR surface;
VkDevice device;
VkQueue queue;
VkPhysicalDevice physicalDevice = NULL;
// physical device properties
VkPhysicalDeviceProperties deviceProperties;
VkPhysicalDeviceFeatures deviceFeatures;
VkPhysicalDeviceMemoryProperties deviceMemProperties;
// index of the queue family we're using
uint32_t queueFamilyIndex;
// surface properties
VkSurfaceCapabilitiesKHR surfaceCapabilities;
VkSurfaceFormatKHR surfaceFormat;
VkPresentModeKHR surfacePresentMode;

// render passes
//
VkRenderPass baseRenderPass;
VkRenderPass finalRenderPass;

// image data
//
VkImage prePostProcessImage;
VkDeviceMemory prePostProcessImageMemory;
VkImageView prePostProcessImageView;
VkFramebuffer prePostProcessImageFramebuffer;
VkSampler prePostProcessImageSampler;

VkImage hdrBloomDownsampledImage;
VkDeviceMemory hdrBloomDownsampledImageMemory;
VkImageView hdrBloomDownsampledImageView;
VkFramebuffer hdrBloomDownsampledImageFramebuffer;
VkSampler hdrBloomDownsampledImageSampler;

// swapchain data
//
VkSwapchainKHR swapchain;
VkExtent2D swapchainExtent;
VkCommandPool swapchainCommandPool;
VkCommandBuffer swapchainCommandBuffers[MAX_FRAMES_IN_FLIGHT];
VkSemaphore imageAvailableSemaphores[MAX_FRAMES_IN_FLIGHT];
VkSemaphore renderFinishedSemaphores[MAX_FRAMES_IN_FLIGHT];
VkFence inFlightFences[MAX_FRAMES_IN_FLIGHT];
// the index used for duplicated resources (it is updated im main loop)
uint32_t frame = 0;

// utility data
//
// command pool for single-time-submit command buffers (created with VK_COMMAND_POOL_CREATE_TRANSIENT_BIT flag)
VkCommandPool shortCommandPool;
// clear (background) color value
VkClearValue clearColorValue = {.color = (VkClearColorValue){0.0f, 0.0f, 0.0f, 1.0f}};

// descriptor set layouts
//
VkDescriptorSetLayout staticSpriteDescriptorSetLayout;
VkDescriptorSetLayout animatedSpriteDescriptorSetLayout;
VkDescriptorSetLayout prePostProcessImageDescriptorSetLayout;
VkDescriptorSetLayout hdrBloomDownsampledImageDescriptorSetLayout;

// descriptor pools
//
VkDescriptorPool postProcessDescriptorPool;

// descriptor sets
//
VkDescriptorSet prePostProcessImageDescriptorSet;
VkDescriptorSet hdrBloomDownsampledDescriptorSet;

// pipeline layouts
//
VkPipelineLayout staticSpritePipelineLayout;
VkPipelineLayout animatedSpritePipelineLayout;
VkPipelineLayout hdrBloomUpsamplePipelineLayout;
VkPipelineLayout hdrBloomDownsamplePipelineLayout;

// pipelines
//
VkPipeline staticSpritePipeline;
VkPipeline animatedSpritePipeline;
VkPipeline hdrBloomUpsamplePipeline;
VkPipeline hdrBloomDownsamplePipeline;

// buffers
//
// vertex and index buffers for a quad
VkBuffer quadVertexBuffer;
VkDeviceMemory quadVertexBufferMemory;
VkBuffer quadIndexBuffer;
VkDeviceMemory quadIndexBufferMemory;
// quad buffers data
#define QUAD_VERT_NUM 4
#define QUAD_IDX_NUM 6
vertex quadVertexData[QUAD_VERT_NUM] = {
    {{-1.0f, -1.0f}},
    {{1.0f, -1.0f}},
    {{-1.0f, 1.0f}},
    {{1.0f, 1.0f}}
};
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
// if you're using tessellation shaders, specify this input assembly state
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

