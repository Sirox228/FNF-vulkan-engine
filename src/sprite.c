#include "sprite.h"
#include "vk/descriptor.h"
#include "vk/sampler.h"
#include "vk/vkutil.h"
#include "atlas/xmlatlas.h"
#include <stdlib.h>
#include <cglm/cglm.h>
#include <vulkan/vulkan_core.h>

void staticSpriteCreate(char* imgPath, float x, float y, sprite* pSprite) {
    {
        int32_t w, h, c;
        createImageTexture(imgPath, &pSprite->image, &pSprite->imageMemory, &pSprite->imageView, &w, &h, &c);
        pSprite->viewport.width = (float)w;
        pSprite->viewport.height = (float)h;
        pSprite->scaleX = pSprite->scaleY = 1.0;
    }

    pSprite->viewport.x = x;
    pSprite->viewport.y = y;
    pSprite->viewport.minDepth = 0.0f;
    pSprite->viewport.maxDepth = 1.0f;

    pSprite->scissor.offset = (VkOffset2D){0, 0};
    pSprite->scissor.extent = swapchainExtent;

    createSampler(&pSprite->sampler);

    VkDescriptorPoolSize poolSize = {};
    createDescriptorPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, &poolSize);
    createDescriptorPool(&poolSize, 1, &pSprite->descriptorPool);
    
    createDescriptorSets(pSprite->descriptorPool, 1, &staticSpriteDescriptorSetLayout, &pSprite->descriptorSet);

    VkWriteDescriptorSet descriptorWrite = {};
    VkDescriptorImageInfo descriptorImage = {};
    createDescriptorImageInfo(pSprite->sampler, pSprite->imageView, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, &descriptorImage);
    createWriteDescriptorSet(pSprite->descriptorSet, 0, 1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, &descriptorImage, NULL, &descriptorWrite);

    vkUpdateDescriptorSets(device, 1, &descriptorWrite, 0, NULL);

    pSprite->isAnimated = 0;
}

void staticSpriteDestroy(sprite* pSprite) {
    vkDestroyDescriptorPool(device, pSprite->descriptorPool, NULL);
    vkDestroySampler(device, pSprite->sampler, NULL);
    vkDestroyImageView(device, pSprite->imageView, NULL);
    vkDestroyImage(device, pSprite->image, NULL);
    vkFreeMemory(device, pSprite->imageMemory, NULL);
}

void animatedSpriteCreate(char* imgPath, char* xmlPath, float x, float y, sprite* pSprite) {
    {
        int32_t w, h, c;
        createImageTexture(imgPath, &pSprite->image, &pSprite->imageMemory, &pSprite->imageView, &w, &h, &c);
        pSprite->viewport.width = (float)w;
        pSprite->viewport.height = (float)h;
        pSprite->scaleX = pSprite->scaleY = 1.0;
    }

    pSprite->viewport.x = x;
    pSprite->viewport.y = y;
    pSprite->viewport.minDepth = 0.0f;
    pSprite->viewport.maxDepth = 1.0f;

    pSprite->scissor.offset = (VkOffset2D){0, 0};
    pSprite->scissor.extent = swapchainExtent;

    createSampler(&pSprite->sampler);

    pSprite->atlas = parseXmlAtlas(xmlPath, pSprite->viewport.width, pSprite->viewport.height);
    pSprite->animIndex = 0;
    pSprite->animFrame = 0;
    pSprite->framesOffset = 0;
    pSprite->delay = 0;
    pSprite->accumulator = 0;
    pSprite->isAnimated = 1;

    {
        uint32_t tcbufferSize = 0;
        for (uint32_t i = 0; i < pSprite->atlas.acount; i++) {
            tcbufferSize += pSprite->atlas.animations[i].fcount;
        }
        vec2 tcbuffer[4 * tcbufferSize];

        uint32_t curidx = 0;
        for (uint32_t i = 0; i < pSprite->atlas.acount; i++) {
            for (uint32_t j = 0; j < pSprite->atlas.animations[i].fcount; j++) {
                tcbuffer[curidx][0] = pSprite->atlas.animations[i].frames[j].texcoords[0][0];
                tcbuffer[curidx][1] = pSprite->atlas.animations[i].frames[j].texcoords[0][1];
                curidx++;
                tcbuffer[curidx][0] = pSprite->atlas.animations[i].frames[j].texcoords[1][0];
                tcbuffer[curidx][1] = pSprite->atlas.animations[i].frames[j].texcoords[1][1];
                curidx++;
                tcbuffer[curidx][0] = pSprite->atlas.animations[i].frames[j].texcoords[2][0];
                tcbuffer[curidx][1] = pSprite->atlas.animations[i].frames[j].texcoords[2][1];
                curidx++;
                tcbuffer[curidx][0] = pSprite->atlas.animations[i].frames[j].texcoords[3][0];
                tcbuffer[curidx][1] = pSprite->atlas.animations[i].frames[j].texcoords[3][1];
                curidx++;
            }
        }

        createAndFillBufferFromStaging(4 * tcbufferSize * sizeof(vec2), tcbuffer, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &pSprite->texcoordBuffer, &pSprite->texcoordBufferMemory);

        for (uint32_t i = 0; i < pSprite->atlas.acount; i++) {
            for (uint32_t j = 0; j < pSprite->atlas.animations[i].fcount; j++) {
                free(pSprite->atlas.animations[i].frames[j].texcoords);
            }
        }
    }

    VkDescriptorPoolSize imagePoolSize = {}, bufferPoolSize = {};
    createDescriptorPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, &imagePoolSize);
    createDescriptorPoolSize(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1, &bufferPoolSize);
    createDescriptorPool((VkDescriptorPoolSize[]){imagePoolSize, bufferPoolSize}, 2, &pSprite->descriptorPool);
    
    createDescriptorSets(pSprite->descriptorPool, 1, &animatedSpriteDescriptorSetLayout, &pSprite->descriptorSet);

    VkWriteDescriptorSet descriptorImageWrite = {};
    VkDescriptorImageInfo descriptorImage = {};
    createDescriptorImageInfo(pSprite->sampler, pSprite->imageView, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, &descriptorImage);
    createWriteDescriptorSet(pSprite->descriptorSet, 0, 1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, &descriptorImage, NULL, &descriptorImageWrite);
    
    VkWriteDescriptorSet descriptorBufferWrite = {};
    VkDescriptorBufferInfo descriptorBuffer = {};
    createDescriptorBufferInfo(pSprite->texcoordBuffer, 0, VK_WHOLE_SIZE, &descriptorBuffer);
    createWriteDescriptorSet(pSprite->descriptorSet, 1, 1, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, NULL, &descriptorBuffer, &descriptorBufferWrite);

    vkUpdateDescriptorSets(device, 2, (VkWriteDescriptorSet[]){descriptorImageWrite, descriptorBufferWrite}, 0, NULL);
}


void animatedSpriteDestroy(sprite* pSprite) {
    staticSpriteDestroy(pSprite);
    vkDestroyBuffer(device, pSprite->texcoordBuffer, NULL);
    vkFreeMemory(device, pSprite->texcoordBufferMemory, NULL);
    for (uint32_t i = 0; i < pSprite->atlas.acount; i++) {
        free(pSprite->atlas.animations[i].frames);
    }
    free(pSprite->atlas.animations);
}

void spritePlayAnimation(const char* name, sprite* pSprite) {
    uint32_t hash = stringHash(name, strlen(name));
    uint32_t offset = 0;
    for (uint32_t i = 0; i < pSprite->atlas.acount; i++) {
        if (pSprite->atlas.animations[i].nameHash == hash) {
            pSprite->animIndex = i;
            pSprite->framesOffset = offset;
            break;
        }
        offset += pSprite->atlas.animations[i].fcount;
    }
}

animationHash spriteAnimationHash(const char* name, sprite* pSprite) {
    animationHash hash;
    hash.nameHash = stringHash(name, strlen(name));
    uint32_t offset = 0;
    for (uint32_t i = 0; i < pSprite->atlas.acount; i++) {
        if (pSprite->atlas.animations[i].nameHash == hash.nameHash) {
            hash.animIndex = i;
            hash.framesOffset = offset;
            break;
        }
        offset += pSprite->atlas.animations[i].fcount;
    }
    return hash;
}

void spritePlayAnimationFromHash(animationHash hashStruct, sprite* pSprite) {
    pSprite->animIndex = hashStruct.animIndex;
    pSprite->framesOffset = hashStruct.framesOffset;
}

void spriteSetFps(float fps, sprite* pSprite) {
    pSprite->delay = 1000.0f / fps;
}
