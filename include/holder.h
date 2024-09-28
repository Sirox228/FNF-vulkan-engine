#ifndef HOLDER_H
#define HOLDER_H

#include <SDL.h>
#include <ma/miniaudio.h>
#include "vk/vkFunctions.h"
#include "vk/vertex.h"
#include "config.h"
#include "sprite.h"

// game global variables
//
// flag which closes the game if set to 0
extern uint8_t active;
// time passed since the last frame (time delta)
extern float timeDelta;

extern sprite* sprites;
extern uint32_t globalSpriteCount;
extern void (*stateCreate)();
extern void (*stateEvent)(SDL_Event* event);
extern void (*stateUpdate)();
extern void (*stateDestroy)();

// conductor data
// time since last conductor reset
extern float conductorTime;
// bpm
extern float bpm;
// song positions
extern float songPosition;
extern float currentBeat;
extern float currentStep;
// duration
extern float beatLengthMs;
extern float stepLengthMs;

// sdl window handle
//
extern SDL_Window* window;

// miniaudio global data
//
extern ma_decoder maDecoder;
extern ma_device maDevice;
// device properties
extern ma_format outputFormat;
extern ma_uint32 outputChannels;
extern ma_uint32 outputSampleRate;

// vulkan global data
//
extern VkInstance instance;
extern VkSurfaceKHR surface;
extern VkDevice device;
extern VkQueue queue;
extern VkPhysicalDevice physicalDevice;
// physical device properties
extern VkPhysicalDeviceProperties deviceProperties;
extern VkPhysicalDeviceFeatures deviceFeatures;
extern VkPhysicalDeviceMemoryProperties deviceMemProperties;
// index of the queue family we're using
extern uint32_t queueFamilyIndex;
// surface properties
extern VkSurfaceCapabilitiesKHR surfaceCapabilities;
extern VkSurfaceFormatKHR surfaceFormat;
extern VkPresentModeKHR surfacePresentMode;

// render passes
//
extern VkRenderPass baseRenderPass;
extern VkRenderPass finalRenderPass;

// image data 
//
extern VkImage prePostProcessImage;
extern VkDeviceMemory prePostProcessImageMemory;
extern VkImageView prePostProcessImageView;
extern VkFramebuffer prePostProcessImageFramebuffer;
extern VkSampler prePostProcessImageSampler;

extern VkImage hdrBloomDownsampledImage;
extern VkDeviceMemory hdrBloomDownsampledImageMemory;
extern VkImageView hdrBloomDownsampledImageView;
extern VkFramebuffer hdrBloomDownsampledImageFramebuffer;
extern VkSampler hdrBloomDownsampledImageSampler;

// swapchain data
//
extern VkSwapchainKHR swapchain;
extern VkExtent2D swapchainExtent;
extern VkCommandPool swapchainCommandPool;
extern VkCommandBuffer swapchainCommandBuffers[MAX_FRAMES_IN_FLIGHT];
extern VkSemaphore imageAvailableSemaphores[MAX_FRAMES_IN_FLIGHT];
extern VkSemaphore renderFinishedSemaphores[MAX_FRAMES_IN_FLIGHT];
extern VkFence inFlightFences[MAX_FRAMES_IN_FLIGHT];
// the index used for duplicated resources (it is updated im main loop)
extern uint32_t frame;

// utility data
//
// command pool for single-time-submit command buffers (created with VK_COMMAND_POOL_CREATE_TRANSIENT_BIT flag)
extern VkCommandPool shortCommandPool;
// clear (background) color value
extern VkClearValue clearColorValue;

// descriptor set layouts
//
extern VkDescriptorSetLayout staticSpriteDescriptorSetLayout;
extern VkDescriptorSetLayout animatedSpriteDescriptorSetLayout;
extern VkDescriptorSetLayout prePostProcessImageDescriptorSetLayout;
extern VkDescriptorSetLayout hdrBloomDownsampledImageDescriptorSetLayout;

// descriptor pools
//
extern VkDescriptorPool postProcessDescriptorPool;

// descriptor sets
//
extern VkDescriptorSet prePostProcessImageDescriptorSet;
extern VkDescriptorSet hdrBloomDownsampledDescriptorSet;

// pipeline layouts
//
extern VkPipelineLayout staticSpritePipelineLayout;
extern VkPipelineLayout animatedSpritePipelineLayout;
extern VkPipelineLayout hdrBloomUpsamplePipelineLayout;
extern VkPipelineLayout hdrBloomDownsamplePipelineLayout;

// pipelines
//
extern VkPipeline staticSpritePipeline;
extern VkPipeline animatedSpritePipeline;
extern VkPipeline hdrBloomUpsamplePipeline;
extern VkPipeline hdrBloomDownsamplePipeline;

// buffers
//
// vertex and index buffers for a quad
extern VkBuffer quadVertexBuffer;
extern VkDeviceMemory quadVertexBufferMemory;
extern VkBuffer quadIndexBuffer;
extern VkDeviceMemory quadIndexBufferMemory;
// quad buffers data
#define QUAD_VERT_NUM 4
#define QUAD_IDX_NUM 6
extern vertex quadVertexData[QUAD_VERT_NUM];
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
// if you're using tessellation shaders, specify this input assembly state
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

#endif
