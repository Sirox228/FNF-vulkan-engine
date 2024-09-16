#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>

char* getFileContent(char* path, uint32_t* pContentSize);
uint32_t clamp(uint32_t value, uint32_t min, uint32_t max);

#endif
