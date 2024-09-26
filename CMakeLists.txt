cmake_minimum_required(VERSION 3.20)
project(FNF-vulkan-engine LANGUAGES C VERSION 1.0)

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

    ${CMAKE_CURRENT_LIST_DIR}/src/states/titlestate.c

    #audio
    ${CMAKE_CURRENT_LIST_DIR}/src/ma/audio.c
    
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
find_package(EXPAT REQUIRED)

add_executable(FNF-vulkan-engine ${SOURCES})
target_link_libraries(FNF-vulkan-engine ${SDL2_LIBRARIES} ${EXPAT_LIBRARIES} Vulkan::Headers cglm::cglm)
if(UNIX)
    target_link_libraries(FNF-vulkan-engine m)
endif()
if(NOT VK_NO_PROTOTYPES)
    target_link_libraries(FNF-vulkan-engine Vulkan::Vulkan)
endif()
target_include_directories(FNF-vulkan-engine PUBLIC include)
target_include_directories(FNF-vulkan-engine PUBLIC external/stbi/include)
target_include_directories(FNF-vulkan-engine PUBLIC external/ma/include)

file(COPY ${CMAKE_CURRENT_LIST_DIR}/assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
