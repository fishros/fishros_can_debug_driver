#ifndef __BOTMSG_H__
#define __BOTMSG_H__

#include "Arduino.h"

typedef struct {
    int32_t x=0;     // x-coordinate in mm
    int32_t y=0;     // y-coordinate in mm
    int16_t angle=0; // angle in 0.1 degrees
}__attribute__((packed)) Pose; // 4+4+2=10


#endif