#ifndef __UTILS_H__
#define __UTILS_H__
#include <math.h>
#include "botlog.h"
#include "WiFi.h"

String getBoardName();

#define DISTANCE_BETWEEN_POINTS(x1, y1, x2, y2) ((int16_t)(sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1))))

#define COMBINE_UINT8_TO_INT16(highByte, lowByte) ((int16_t)(((uint16_t)(highByte) << 8) | (lowByte)))
#define COMBINE_UINT8_TO_UINT16(highByte, lowByte) ((uint16_t)(((uint16_t)(highByte) << 8) | (lowByte)))

#define CONVERT_UINT8_TO_UINT32(byte0, byte1, byte2, byte3) \
    ((uint32_t)(byte0) << 24 | \
     (uint32_t)(byte1) << 16 | \
     (uint32_t)(byte2) << 8 | \
     (uint32_t)(byte3))

#define DEG2RAD(degrees) ((degrees) * DEG_TO_RAD)               // degrees * (PI / 180.0)

#define RAD2DEG(radians) ((radians) * RAD_TO_DEG)               // radians * (180.0 / PI)

#define RAD2DEGINT16(radians) (int16_t)((radians) * RAD_TO_DEG) // radians * (180.0 / PI)

#define ATAN2_DIRECTION(target_x, target_y, current_x, current_y) \
    atan2((double)((target_y) - (current_y)), (double)((target_x) - (current_x)))

// 高八位低八位互换
#define SWAP_HIGH_LOW_8BIT(x) ((((x) & 0xFF) << 8) | (((x) >> 8) & 0xFF))

#define RCSOFTCHECK(fn)                                                                           \
    {                                                                                             \
        rcl_ret_t temp_rc = fn;                                                                   \
        if ((temp_rc != RCL_RET_OK))                                                              \
        {                                                                                         \
            botlog_debug_name("ros2",                                                                 \
                          "Failed status on line %d: %d. Continuing.\n", __LINE__, (int)temp_rc); \
        }                                                                                         \
    }

#define EXECUTE_EVERY_N_MS(MS, X)          \
    do                                     \
    {                                      \
        static volatile int64_t init = -1; \
        if (init == -1)                    \
        {                                  \
            init = millis();               \
        }                                  \
        if (millis() - init > MS)          \
        {                                  \
            X;                             \
            init = millis();               \
        }                                  \
    } while (0);




#define RUN_EVERY(interval, code)                   \
    do                                              \
    {                                               \
        static unsigned long lastMillis = 0;        \
        unsigned long currentMillis = millis();     \
        if (currentMillis - lastMillis >= interval) \
        {                                           \
            lastMillis = currentMillis;             \
            {                                       \
                code                                \
            }                                       \
        }                                           \
    } while (0)

#define ANGLE_DIFF_INT16_180(target, current) \
    ({                                        \
        int16_t diff = (target) - (current);  \
        if (diff < -180)                      \
        {                                     \
            diff += 360;                      \
        }                                     \
        if (diff > 180)                       \
        {                                     \
            diff -= 360;                      \
        }                                     \
        diff;                                 \
    })

#define ANGLE_DIFF_INT16_1800(target, current) \
    ({                                         \
        int16_t diff = (target) - (current);   \
        if (diff < -1800)                      \
        {                                      \
            diff += 3600;                      \
        }                                      \
        if (diff > 1800)                       \
        {                                      \
            diff -= 3600;                      \
        }                                      \
        diff;                                  \
    })

#define LIMIT_SIGNED_INT16_T(value, min, max) \
    do                                        \
    {                                         \
        if ((value) > 0)                     \
        {                                     \
            if ((value) < (min))              \
            {                                 \
                (value) = (min);              \
            }                                 \
            else if ((value) > (max))         \
            {                                 \
                (value) = (max);              \
            }                                 \
        }                                     \
        else if((value)<0)                                  \
        {                                     \
            if ((value) > (-min))             \
            {                                 \
                (value) = (-min);             \
            }                                 \
            else if ((value) < (-max))        \
            {                                 \
                (value) = (-max);             \
            }                                 \
        }                                     \
    } while (0)

/**
 * @brief 将数据帧打印成hex形式
 *
 * @param title 数据标题
 * @param buffer 数据
 * @param size 长度
 */
void printFrame2Hex(const char *title, uint8_t *buffer, uint16_t size);

uint16_t calculateCRC16(uint8_t *data, size_t length);
uint8_t checkSum8(uint8_t crc, const uint8_t *data, size_t length);
uint16_t checkSum16(uint16_t crc, const uint8_t *data, size_t length);

uint8_t *allocDMABuffer(const size_t n);

int findEndIndex(const uint8_t *frame, int startIndex, int endIndex);
int findStartIndex(const uint8_t *frame, int startIndex, int endIndex);

int findStartIndex5A(const uint8_t *frame, int startIndex, int endIndex);
int findEndIndex5A(const uint8_t *frame, int startIndex, int endIndex);

int integerSquareRoot(int n);
int inverse_escape_frame(uint8_t *frame, uint8_t *result, uint16_t len);

int escape_frame(uint8_t *frame, uint8_t *result, uint16_t len);

#endif