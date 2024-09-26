#ifndef XML_ATLAS_H
#define XML_ATLAS_H

#include <stdint.h>
#include <cglm/cglm.h>

typedef struct animframe {
    uint32_t w, h;
    int32_t fx, fy;
    vec2* texcoords;
} animframe;

typedef struct animation {
    uint32_t nameHash;
    animframe* frames;
    uint32_t fsize;
    uint32_t fcount;
} animation;

typedef struct sparrowAtlas {
    animation* animations;
    uint32_t asize;
    uint32_t acount;
} sparrowAtlas;

sparrowAtlas parseXmlAtlas(char* path, float w, float h);
uint32_t stringHash(const char* str, uint32_t len);

#endif
