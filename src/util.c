#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

char* getFileContent(char* path, uint32_t* pContentSize) {
    FILE* file = fopen(path, "rb");
    fseek(file, 0, SEEK_END);
    uint32_t fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* content = malloc(sizeof(char) * fileSize);
    fread(content, fileSize, 1, file);
    fclose(file);

    *pContentSize = fileSize;

    return content;
}

uint32_t clamp(uint32_t value, uint32_t min, uint32_t max) {
    if (value <= min) {
        return min;
    }
    if (value >= max) {
        return max;
    }
    return value;
}
