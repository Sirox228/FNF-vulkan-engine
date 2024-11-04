cmake_minimum_required(VERSION 3.25)
project(VkFunkin LANGUAGES C VERSION 1.0)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

option(VALIDATION "enable validation layer" OFF)
option(VK_NO_PROTOTYPES "when enabled, vulkan functions will be loaded at runtime instead of linking with vulkan library at compile-time" ON)

if(VALIDATION)
    add_compile_definitions(VALIDATION)
endif()

if(VK_NO_PROTOTYPES)
    add_compile_definitions(VK_NO_PROTOTYPES)
endif()

add_compile_definitions(CGLM_FORCE_DEPTH_ZERO_TO_ONE)

set(SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/src/main.c
    ${CMAKE_CURRENT_LIST_DIR}/src/holder.c
    ${CMAKE_CURRENT_LIST_DIR}/src/util.c
    ${CMAKE_CURRENT_LIST_DIR}/src/render.c
    ${CMAKE_CURRENT_LIST_DIR}/src/sprite.c
    ${CMAKE_CURRENT_LIST_DIR}/src/atlas/xmlatlas.c

    #frontend
    ${CMAKE_CURRENT_LIST_DIR}/src/funkin/states/titlestate.c
    ${CMAKE_CURRENT_LIST_DIR}/src/funkin/conductor.c
    
    #vulkan stuff
    ${CMAKE_CURRENT_LIST_DIR}/src/vk/vkFunctions.c
    ${CMAKE_CURRENT_LIST_DIR}/src/vk/instance.c
    ${CMAKE_CURRENT_LIST_DIR}/src/vk/surface.c
    ${CMAKE_CURRENT_LIST_DIR}/src/vk/vkutil.c
    ${CMAKE_CURRENT_LIST_DIR}/src/vk/physicalDevice.c
    ${CMAKE_CURRENT_LIST_DIR}/src/vk/device.c
    ${CMAKE_CURRENT_LIST_DIR}/src/vk/swapchain.c
    ${CMAKE_CURRENT_LIST_DIR}/src/vk/renderPass.c
    ${CMAKE_CURRENT_LIST_DIR}/src/vk/shader.c
    ${CMAKE_CURRENT_LIST_DIR}/src/vk/pipeline.c
    ${CMAKE_CURRENT_LIST_DIR}/src/vk/commandPool.c
    ${CMAKE_CURRENT_LIST_DIR}/src/vk/commandBuffer.c
    ${CMAKE_CURRENT_LIST_DIR}/src/vk/framebuffer.c
    ${CMAKE_CURRENT_LIST_DIR}/src/vk/sync.c
    ${CMAKE_CURRENT_LIST_DIR}/src/vk/descriptor.c
    ${CMAKE_CURRENT_LIST_DIR}/src/vk/sampler.c
    ${CMAKE_CURRENT_LIST_DIR}/src/vk/image.c
    ${CMAKE_CURRENT_LIST_DIR}/src/vk/imageView.c
    ${CMAKE_CURRENT_LIST_DIR}/src/vk/buffer.c

    #externals
    ${CMAKE_CURRENT_LIST_DIR}/external/stbi/src/stb_image.c
    ${CMAKE_CURRENT_LIST_DIR}/external/ma/src/miniaudio.c
)

find_package(SDL2 REQUIRED)
find_package(Vulkan REQUIRED)
find_package(cglm REQUIRED)
find_package(expat REQUIRED)

add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})
target_link_libraries(${CMAKE_PROJECT_NAME} SDL2::SDL2 expat::expat Vulkan::Headers cglm::cglm)
if(UNIX)
    target_link_libraries(${CMAKE_PROJECT_NAME} m)
endif()
if(NOT VK_NO_PROTOTYPES)
    target_link_libraries(${CMAKE_PROJECT_NAME} Vulkan::Vulkan)
endif()
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC include external/stbi/include external/ma/include)

file(COPY ${CMAKE_CURRENT_LIST_DIR}/assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
