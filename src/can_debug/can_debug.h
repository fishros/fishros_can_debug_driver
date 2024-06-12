#ifndef __CAN_DEBUG_H__
#define __CAN_DEBUG_H__
#include <ETH.h> //引用以使用ETH
#include <botlog.h>
#include <utils.h>
#include "robot_config.h"
#include "protocdef.h"
#include "utils.h"
#include "robot_config.h"

#include <ESP32-TWAI-CAN.hpp>

void can_debug_task(void* param);


#endif // __CAN_DEBUG_H__