#ifndef SPRITE_H
#define SPRITE_H

#include "holder.h"
#include "atlas/xmlatlas.h"

typedef struct animationHash {
    uint32_t framesOffset;
    uint32_t animIndex;
    uint32_t nameHash;
} animationHash;

typedef struct sprite {
    float scaleX, scaleY;
    VkViewport viewport;
    VkRect2D scissor;
    VkImage image;
    VkDeviceMemory imageMemory;
    VkImageView imageView;
    VkSampler sampler;
    VkDescriptorPool descriptorPool;
    VkDescriptorSet descriptorSet;

    uint8_t isAnimated;

    // animated stuff
    VkBuffer texcoordBuffer;
    VkDeviceMemory texcoordBufferMemory;
    sparrowAtlas atlas;
    float delay;
    float accumulator;
    uint32_t animFrame;
    uint32_t framesOffset;
    uint32_t animIndex;
} sprite;

sprite staticSpriteCreate(char* imgPath, float x, float y);
void staticSpriteDestroy(sprite* pSprite);
sprite animatedSpriteCreate(char* imgPath, char* xmlPath, float x, float y, float fps);
void animatedSpriteDestroy(sprite* pSprite);
void spritePlayAnimation(const char* name, sprite* pSprite);
animationHash spriteAnimationHash(const char* name, sprite* pSprite);
void spritePlayAnimationFromHash(animationHash hashStruct, sprite* pSprite);
void spriteSetFps(float fps, sprite* pSprite);

#endif
