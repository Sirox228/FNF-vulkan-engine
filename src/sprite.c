#include "holder.h"
#include "sprite.h"
#include "vk/descriptor.h"
#include "vk/sampler.h"
#include "vk/vkutil.h"
#include "vk/buffer.h"
#include "vk/image.h"
#include "atlas/xmlatlas.h"
#include <stdlib.h>
#include <cglm/cglm.h>

sprite staticSpriteCreate(char* imgPath, float x, float y) {
	sprite spr;
	
    {
        int32_t w, h, c;
        createImageTexture(imgPath, &spr.image, &spr.imageMemory, &spr.imageView, &w, &h, &c);
        spr.viewport.width = (float)w;
        spr.viewport.height = (float)h;
        spr.scaleX = spr.scaleY = 1.0;
    }

    spr.viewport.x = x;
    spr.viewport.y = y;
    spr.viewport.minDepth = 0.0f;
    spr.viewport.maxDepth = 1.0f;

    spr.scissor.offset = (VkOffset2D){0, 0};
    spr.scissor.extent = swapchainExtent;

    createSampler(VK_FILTER_LINEAR, VK_FILTER_LINEAR, &spr.sampler);

    VkDescriptorPoolSize poolSize = {};
    createDescriptorPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, &poolSize);
    createDescriptorPool(1, &poolSize, 1, &spr.descriptorPool);
    
    createDescriptorSets(spr.descriptorPool, 1, &staticSpriteDescriptorSetLayout, &spr.descriptorSet);

    VkWriteDescriptorSet descriptorWrite = {};
    VkDescriptorImageInfo descriptorImage = {};
    createDescriptorImageInfo(spr.sampler, spr.imageView, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, &descriptorImage);
    createWriteDescriptorSet(spr.descriptorSet, 0, 1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, &descriptorImage, NULL, &descriptorWrite);

    vkUpdateDescriptorSets(device, 1, &descriptorWrite, 0, NULL);

    spr.isAnimated = 0;
    return spr;
}

void staticSpriteDestroy(sprite* pSprite) {
    vkDestroyDescriptorPool(device, pSprite->descriptorPool, NULL);
    vkDestroySampler(device, pSprite->sampler, NULL);
    vkDestroyImageView(device, pSprite->imageView, NULL);
    vkDestroyImage(device, pSprite->image, NULL);
    vkFreeMemory(device, pSprite->imageMemory, NULL);
}

sprite animatedSpriteCreate(char* imgPath, char* xmlPath, float x, float y, float fps) {
	sprite spr;
	
    {
        int32_t w, h, c;
        createImageTexture(imgPath, &spr.image, &spr.imageMemory, &spr.imageView, &w, &h, &c);
        spr.viewport.width = (float)w;
        spr.viewport.height = (float)h;
        spr.scaleX = spr.scaleY = 1.0;
    }

    spr.viewport.x = x;
    spr.viewport.y = y;
    spr.viewport.minDepth = 0.0f;
    spr.viewport.maxDepth = 1.0f;

    spr.scissor.offset = (VkOffset2D){0, 0};
    spr.scissor.extent = swapchainExtent;

    createSampler(VK_FILTER_LINEAR, VK_FILTER_LINEAR, &spr.sampler);

    spr.atlas = parseXmlAtlas(xmlPath, spr.viewport.width, spr.viewport.height);
    spr.animIndex = 0;
    spr.animFrame = 0;
    spr.framesOffset = 0;
    spr.delay = 0;
    spr.accumulator = 0;
    spr.isAnimated = 1;
    spr.loopAnimation = 1;

    {
        uint32_t tcbufferSize = 0;
        for (uint32_t i = 0; i < spr.atlas.acount; i++) {
            tcbufferSize += spr.atlas.animations[i].fcount;
        }
        vec2 tcbuffer[4 * tcbufferSize];

        uint32_t curidx = 0;
        for (uint32_t i = 0; i < spr.atlas.acount; i++) {
            for (uint32_t j = 0; j < spr.atlas.animations[i].fcount; j++) {
                tcbuffer[curidx][0] = spr.atlas.animations[i].frames[j].texcoords[0][0];
                tcbuffer[curidx][1] = spr.atlas.animations[i].frames[j].texcoords[0][1];
                curidx++;
                tcbuffer[curidx][0] = spr.atlas.animations[i].frames[j].texcoords[1][0];
                tcbuffer[curidx][1] = spr.atlas.animations[i].frames[j].texcoords[1][1];
                curidx++;
                tcbuffer[curidx][0] = spr.atlas.animations[i].frames[j].texcoords[2][0];
                tcbuffer[curidx][1] = spr.atlas.animations[i].frames[j].texcoords[2][1];
                curidx++;
                tcbuffer[curidx][0] = spr.atlas.animations[i].frames[j].texcoords[3][0];
                tcbuffer[curidx][1] = spr.atlas.animations[i].frames[j].texcoords[3][1];
                curidx++;
            }
        }

        createAndFillBufferFromStaging(4 * tcbufferSize * sizeof(vec2), tcbuffer, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &spr.texcoordBuffer, &spr.texcoordBufferMemory);

        for (uint32_t i = 0; i < spr.atlas.acount; i++) {
            for (uint32_t j = 0; j < spr.atlas.animations[i].fcount; j++) {
                free(spr.atlas.animations[i].frames[j].texcoords);
            }
        }
    }

    VkDescriptorPoolSize imagePoolSize = {}, bufferPoolSize = {};
    createDescriptorPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, &imagePoolSize);
    createDescriptorPoolSize(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1, &bufferPoolSize);
    createDescriptorPool(1, (VkDescriptorPoolSize[]){imagePoolSize, bufferPoolSize}, 2, &spr.descriptorPool);
    
    createDescriptorSets(spr.descriptorPool, 1, &animatedSpriteDescriptorSetLayout, &spr.descriptorSet);

    VkWriteDescriptorSet descriptorImageWrite = {};
    VkDescriptorImageInfo descriptorImage = {};
    createDescriptorImageInfo(spr.sampler, spr.imageView, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, &descriptorImage);
    createWriteDescriptorSet(spr.descriptorSet, 0, 1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, &descriptorImage, NULL, &descriptorImageWrite);
    
    VkWriteDescriptorSet descriptorBufferWrite = {};
    VkDescriptorBufferInfo descriptorBuffer = {};
    createDescriptorBufferInfo(spr.texcoordBuffer, 0, VK_WHOLE_SIZE, &descriptorBuffer);
    createWriteDescriptorSet(spr.descriptorSet, 1, 1, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, NULL, &descriptorBuffer, &descriptorBufferWrite);

    vkUpdateDescriptorSets(device, 2, (VkWriteDescriptorSet[]){descriptorImageWrite, descriptorBufferWrite}, 0, NULL);
    
    spriteSetFps(fps, &spr);
    
    return spr;
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
