/**
 * @file botlog.h
 * @author fishros (fishros@foxmail.com)
 * @brief 日志管理类
 * @version V1.0.0
 * @date 2023-01-04
 *
 * @copyright Copyright (c) tonsincs.com 2023
 *
 */
#ifndef __UTILS_BOTLOG_H__
#define __UTILS_BOTLOG_H__

#include "Arduino.h"
#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/uart.h"
#include "esp_log.h"

#define LOG_LEVEL_DEBUG 0
#define LOG_LEVEL_INFO 1
#define LOG_LEVEL_WARN 2
#define LOG_LEVEL_NONE 8

class BotLog
{
private:
    char log_buff[512];
    Stream *target{nullptr};
    BotLog() = default;

public:
    static BotLog &getInstance()
    {
        static BotLog instance;
        return instance;
    }
    void setLogTarget(Stream &stream)
    {
        target = &stream;
    };
    ~BotLog() = default;
    BotLog(const BotLog &) = delete;
    BotLog &operator=(const BotLog &) = delete;

public:
    int log(uint8_t level, const char *format, ...)
    {
        if (target != nullptr)
        {
            char loc_buf[64];
            char *temp = loc_buf;
            va_list arg;
            va_list copy;
            va_start(arg, format);
            va_copy(copy, arg);
            int len = vsnprintf(temp, sizeof(loc_buf), format, copy);
            va_end(copy);
            if (len < 0)
            {
                va_end(arg);
                return 0;
            };
            if (len >= sizeof(loc_buf))
            {
                temp = (char *)malloc(len + 1);
                if (temp == NULL)
                {
                    va_end(arg);
                    return 0;
                }
                len = vsnprintf(temp, len + 1, format, arg);
            }
            va_end(arg);
            len = target->write((uint8_t *)temp, len);
            // len = uart_write_bytes(UART_NUM_1, (const char *)temp, len);

            
            if (temp != loc_buf)
            {
                free(temp);
            }
            return len;
        }
        return -1;
    }
};

#define botlog_set_target(stream) BotLog::getInstance().setLogTarget(stream);

#define botlog_debug_name(log_name, fmt, arg...)                                                                         \
    do                                                                                                                   \
    {                                                                                                                    \
        if (ENABLE_LOG)                                                                                                  \
            BotLog::getInstance().log(LOG_LEVEL_DEBUG, "[%d][D:]@%s:%d>" fmt "\n", millis(), log_name, __LINE__, ##arg); \
    } while (0);

#define botlog_debug(fmt, arg...)                                                                                            \
    do                                                                                                                       \
    {                                                                                                                        \
        if (ENABLE_LOG)                                                                                                      \
            BotLog::getInstance().log(LOG_LEVEL_DEBUG, "[%d][D:]@%s:%d>" fmt "\n", millis(), __FILENAME__, __LINE__, ##arg); \
    } while (0);

#define botlog_debug_raw(fmt, arg...)                               \
    do                                                              \
    {                                                               \
        if (ENABLE_LOG)                                             \
            BotLog::getInstance().log(LOG_LEVEL_DEBUG, fmt, ##arg); \
    } while (0);

#define botlog_warn_name(log_name, fmt, arg...)                                                                      \
    do                                                                                                               \
    {                                                                                                                \
        BotLog::getInstance().log(LOG_LEVEL_DEBUG, "[%d][W:]@%s:%d>" fmt "\n", millis(), log_name, __LINE__, ##arg); \
    } while (0);

#define botlog_warn(fmt, arg...)                                                                                         \
    do                                                                                                                   \
    {                                                                                                                    \
        BotLog::getInstance().log(LOG_LEVEL_DEBUG, "[%d][W:]@%s:%d>" fmt "\n", millis(), __FILENAME__, __LINE__, ##arg); \
    } while (0);

#define botlog_info_name(log_name, fmt, arg...)                                                                      \
    do                                                                                                               \
    {                                                                                                                \
        BotLog::getInstance().log(LOG_LEVEL_DEBUG, "[%d][I:]@%s:%d>" fmt "\n", millis(), log_name, __LINE__, ##arg); \
    } while (0);

#define botlog_info(fmt, arg...)                                                                               \
    do                                                                                                                   \
    {                                                                                                                    \
        BotLog::getInstance().log(LOG_LEVEL_DEBUG, "[%d][I:]@%s:%d>" fmt "\n", millis(), __FILENAME__, __LINE__, ##arg); \
    } while (0);

#endif // __UTILS_BOTLOG_H__