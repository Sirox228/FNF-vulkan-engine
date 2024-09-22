#include <expat.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "atlas/xmlatlas.h"

typedef struct parserInput {
    float w, h;
    sparrowAtlas* atlas;
} parserInput;

uint32_t stringHash(const char* str, uint32_t len) {
    uint32_t res = 0;
    for (uint32_t i = 0; i < len; i++) {
        res += str[i];
    }
    return res;
}

void XMLCALL startElementHandler(void* userData, const char* name, const char** atts) {
    if (strcmp(name, "SubTexture") != 0) return;

    #define inp ((parserInput*)userData)

    uint32_t animName = stringHash(atts[1], strlen(atts[1])-4);
    uint32_t animIndex = 0;
    uint8_t found = 0;
    for (uint32_t i = 0; i < inp->atlas->acount; i++) {
        if (inp->atlas->animations[i].nameHash == animName) {
            found = 1;
            animIndex = i;
            break;
        }
    }
    if (!found) {
        if (inp->atlas->asize == inp->atlas->acount) {
            inp->atlas->asize += 10;
            inp->atlas->animations = realloc(inp->atlas->animations, inp->atlas->asize * sizeof(animation));
            for (uint32_t i = inp->atlas->acount; i < inp->atlas->asize; i++) {
                inp->atlas->animations[i].fsize = 10;
                inp->atlas->animations[i].fcount = 0;
                inp->atlas->animations[i].frames = calloc(inp->atlas->animations[i].fsize, sizeof(animframe));
            }
        }
        inp->atlas->animations[inp->atlas->acount].nameHash = animName;
        animIndex = inp->atlas->acount;
        inp->atlas->acount++;
    }

    uint32_t x = atoi(atts[3]),
             y = atoi(atts[5]),
             sw = atoi(atts[7]),
             sh = atoi(atts[9]);

    // calculate normalized texture coordinates
    float normX = x / inp->w,
          normY = y / inp->h,
          normXW = (x + sw) / inp->w,
          normYH = (y + sh) / inp->h;

    uint32_t frameIndex = atoi(atts[1] + strlen(atts[1]) - 4);
    if (frameIndex >= inp->atlas->animations[animIndex].fsize) {
        inp->atlas->animations[animIndex].fsize = frameIndex + 5;
        inp->atlas->animations[animIndex].frames = realloc(inp->atlas->animations[animIndex].frames, inp->atlas->animations[animIndex].fsize * sizeof(animframe));
    }
    inp->atlas->animations[animIndex].frames[frameIndex] = (animframe){sw, sh, NULL};
    inp->atlas->animations[animIndex].frames[frameIndex].texcoords = malloc(4 * sizeof(vec2));
    inp->atlas->animations[animIndex].frames[frameIndex].texcoords[0][0] = normX;
    inp->atlas->animations[animIndex].frames[frameIndex].texcoords[0][1] = normY;
    inp->atlas->animations[animIndex].frames[frameIndex].texcoords[1][0] = normXW;
    inp->atlas->animations[animIndex].frames[frameIndex].texcoords[1][1] = normY;
    inp->atlas->animations[animIndex].frames[frameIndex].texcoords[2][0] = normX;
    inp->atlas->animations[animIndex].frames[frameIndex].texcoords[2][1] = normYH;
    inp->atlas->animations[animIndex].frames[frameIndex].texcoords[3][0] = normXW;
    inp->atlas->animations[animIndex].frames[frameIndex].texcoords[3][1] = normYH;
    inp->atlas->animations[animIndex].fcount++;
}

sparrowAtlas parseXmlAtlas(char* path, float w, float h) {
    char buf[BUFSIZ];

    XML_Parser parser = XML_ParserCreate(NULL);
    XML_SetElementHandler(parser, startElementHandler, NULL);
    FILE* f = fopen(path, "r");

    uint32_t done;
    uint32_t len;

    sparrowAtlas atlas = {};
    atlas.asize = 10;
    atlas.acount = 0;
    atlas.animations = calloc(atlas.asize, sizeof(animation));
    for (uint32_t i = 0; i < atlas.asize; i++) {
        atlas.animations[i].fsize = 10;
        atlas.animations[i].fcount = 0;
        atlas.animations[i].frames = calloc(atlas.animations[i].fsize, sizeof(animframe));
    }

    parserInput input = (parserInput){w, h, &atlas};
    XML_SetUserData(parser, &input);

    do {
        len = (uint32_t)fread(buf, 1, sizeof(buf), f);
        done = len < sizeof(buf);
        if (XML_Parse(parser, buf, len, done) == XML_STATUS_ERROR) {
            printf("failed to parse xml\n");
            exit(0);
        }
    } while (!done);

    fclose(f);
    XML_ParserFree(parser);

    return atlas;
}
