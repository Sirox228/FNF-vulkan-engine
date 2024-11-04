#ifndef CONDUCTOR_H
#define CONDUCTOR_H

#include <stdint.h>

uint32_t getStep();
uint32_t getStepInt();
uint32_t getBeat();
uint32_t getBeatInt();
void setBpm(float newBpm);

#endif
