
#include "robot_config.h"

RobotConfig SystemConfig;
uint32_t RobotConfig::is_first_startup()
{
    // fishlog_debug("config", "first_startup=%d", preferences.getBool("first_startup", true) ? 1 : 0);
    return preferences.getBool("first_startup", true);
}

bool RobotConfig::config(String key, String value)
{
    // fishlog_debug("config", "save config key=%s,value=%s", key.c_str(), value.c_str());
    return preferences.putString(key.c_str(), value.c_str());
}


String RobotConfig::board_name()
{
    static char board_name[24];
    uint8_t macAddr[6];
    WiFi.macAddress(macAddr);
    sprintf(board_name, "ROS2_%02X%02X", macAddr[4], macAddr[5]);
    return String(board_name);
}

/**
 * @brief
 * @param line
 * @param result  1 正常分割，-1 错误分割
 * @return int8_t
 */
int8_t RobotConfig::split_str(const char *line, char result[][32])
{
    if (line[0] != '$')
        return CONFIG_PARSE_ERROR;
    uint16_t index = 0;
    uint16_t count = 0;
    uint16_t temp_index = 0;
    for (index = 1; line[index] != '\0'; index++)
    {
        if (line[index] == '=')
        {
            result[count++][temp_index++] = '\0';
            temp_index = 0;
            continue;
        }
        result[count][temp_index++] = line[index];
    }
    result[count][temp_index++] = '\0';

    if (count != 1)
    {
        return CONFIG_PARSE_ERROR;
    }
    return CONFIG_PARSE_OK;
}

/**
 * @brief
 *
 * @param c
 * @param result  0无数据，1有配置，-1错误解析
 * @return int8_t
 */
int8_t RobotConfig::loop_config_uart(int c, char result[][32])
{
    static char line[512];
    static int index = 0;
    if (c == '\n')
    {
        line[index] = '\0';
        index = 0;
        return split_str(line, result);
    }
    else if (c > 0 && c < 127)
    {
        line[index] = c;
        ++index;
    }
    return CONFIG_PARSE_NODATA;
}


void RobotConfig::deal_command(char key[32], char value[32])
{
    // 检查传入的 "key" 是否为 "command"
    if (strcmp(key, "command") == 0)
    {
        // 检查 "value" 是否为 "restart"
        if (strcmp(value, "restart") == 0)
        {
            // 通过调用 "esp_restart" 函数来重新启动 ESP8266
            esp_restart();
        }
        // 检查 "value" 是否 "read_config"
        else if (strcmp(value, "read_config") == 0)
        {
            // 调用 "config.config_str()" 函数来获取当前配置，并通过串口输出到终端
            // "config.config_str()" 函数的作用是将当前配置转化为一个字符串类型，以便于输出或存储
            Serial.print(config_str());
        }
        return;
    }
    // 传入的命令不是 "restart" 或 "read_config"
    else
    {
        // 创建两个 String 对象 "recv_key" 和 "recv_value"
        // 并将传入的 "key" 和 "value" 分别作为参数来初始化这两个对象。
        String recv_key(key);
        String recv_value(value);
        // 调用 "config.config" 方法，将 "recv_key" 和 "recv_value" 作为参数传递给它，将它们存储到当前配置中
        config(recv_key, recv_value);
        Serial.print("$result=ok\n");
    }
}