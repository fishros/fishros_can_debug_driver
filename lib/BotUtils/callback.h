#ifndef __CALLBACK_H__
#define __CALLBACK_H__

#include "stdio.h"

// 定义结构体，包含指向成员函数的指针以及实例的指针
typedef struct {
    void (*bCallbackFunction)(int, void*); // 函数指针
    void* instance; // 实例指针
} BCallbackStruct;



#endif