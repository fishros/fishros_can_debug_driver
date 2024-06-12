#ifndef __ROBOT_CONFIG_H__
#define __ROBOT_CONFIG_H__

#include <Arduino.h>
#include <Preferences.h>
#include <WiFi.h>

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define DEFINE_GET_CONFIG_INT(CONFIG_NAME, DEFAULT_VALUE)                  \
    int32_t get_config_##CONFIG_NAME()                                     \
    {                                                                      \
        return preferences.getString(#CONFIG_NAME, DEFAULT_VALUE).toInt(); \
    }

#define DEFINE_GET_CONFIG_STR(CONFIG_NAME, DEFAULT_VALUE)          \
    String get_config_##CONFIG_NAME()                              \
    {                                                              \
        return preferences.getString(#CONFIG_NAME, DEFAULT_VALUE); \
    }

#define DEFINE_CONCAT_CONFIG(CONFIG_NAME)             \
    config.concat("\n$" #CONFIG_NAME "=");    \
    config.concat(get_config_##CONFIG_NAME());

/*=========================================常量值定义========================================*/
#define CONFIG_TRANSPORT_MODE_USB "usb"
#define CONFIG_TRANSPORT_MODE_ETH_STATIC "eth_static"
#define CONFIG_TRANSPORT_MODE_ETH_DHCP "eth_dhcp"
#define CONFIG_TRANSPORT_MODE_WIFI_STA "wifi_sta"
#define CONFIG_TRANSPORT_MODE_WIFI_AP "wifi_ap"

/*=========================================默认值定义=====================================*/
#define CONFIG_DEFAULT_TRANSPORT_MODE "eth_static"
#define CONFIG_DEFAULT_SERVER_IP "192.168.168.5" // 默认UDP服务端IP
#define CONFIG_DEFAULT_SERVER_PORT "8888"        // 默认UDP服务端端口号
#define CONFIG_DEFAULT_USB_BAUD "921600"         // usb 协议波特率

#define CONFIG_DEFAULT_ETH_STATIC_IP "192.168.168.250"
#define CONFIG_DEFAULT_ETH_STATIC_GATEWAY "192.168.168.1"
#define CONFIG_DEFAULT_ETH_STATIC_SUBNET "255.255.255.0"
#define CONFIG_DEFAULT_ETH_STATIC_DNS "8.8.8.8"

#define CONFIG_DEFAULT_ROS2_NODE_NAME "protocol_controller"
#define CONFIG_DEFAULT_ROS2_NAMESPACE ""
#define CONFIG_DEFAULT_ROS2_READ_IO_HZ "10" // 读取IO并上报的频率

#define CONFIG_DEFAULT_WIFI_SSID "fishbot"
#define CONFIG_DEFAULT_WIFI_PSWD "12345678"

#define CONFIG_DEFAULT_CAN_RATE "100"      // CAN波特率
#define CONFIG_DEFAULT_485_BAUDRATE "9600" // 485 波特率

#define FIRST_START_TIP "=================================================\n     wwww.fishros.com        \nros2 multi-protocol transport controller v1.0.0\n=================================================\n"

typedef enum
{
    CONFIG_PARSE_ERROR = -1,
    CONFIG_PARSE_NODATA = 0,
    CONFIG_PARSE_OK = 1,
} fishbot_config_status;

class RobotConfig
{
private:
    /* data */
    Preferences preferences;

public:
    uint32_t is_first_startup();

    bool config(String key, String value);
    String board_name();

    void init()
    {
        preferences.begin("fishros");
        if (is_first_startup())
        {
            preferences.putString("transport_mode", CONFIG_DEFAULT_TRANSPORT_MODE);
            preferences.putString("server_ip", CONFIG_DEFAULT_SERVER_IP);
            preferences.putString("server_port", CONFIG_DEFAULT_SERVER_PORT);
            preferences.putString("usb_baudrate", CONFIG_DEFAULT_USB_BAUD);
            
            preferences.putString("eth_ip", CONFIG_DEFAULT_ETH_STATIC_IP);
            preferences.putString("eth_gateway", CONFIG_DEFAULT_ETH_STATIC_GATEWAY);
            preferences.putString("eth_subnet", CONFIG_DEFAULT_ETH_STATIC_SUBNET);
            preferences.putString("eth_dns", CONFIG_DEFAULT_ETH_STATIC_DNS);

            preferences.putString("ros2_namespace", CONFIG_DEFAULT_ROS2_NAMESPACE);
            preferences.putString("ros2_nodename", CONFIG_DEFAULT_ROS2_NODE_NAME);
            preferences.putString("ros2_readio_hz", CONFIG_DEFAULT_ROS2_READ_IO_HZ);
            
            
            preferences.putString("wifi_ssid", CONFIG_DEFAULT_WIFI_SSID);
            preferences.putString("wifi_pswd", CONFIG_DEFAULT_WIFI_PSWD);

            preferences.putString("can_rate", CONFIG_DEFAULT_CAN_RATE);
            preferences.putString("rs485_rate", CONFIG_DEFAULT_485_BAUDRATE);

            preferences.putBool("first_startup", false);
        }
        Serial.println(config_str());
    }
    

    String config_str()
    {
        String config("");
        config.concat("$first_startup=");
        config.concat(is_first_startup());

        DEFINE_CONCAT_CONFIG(transport_mode)
        DEFINE_CONCAT_CONFIG(server_ip)
        DEFINE_CONCAT_CONFIG(server_port)
        DEFINE_CONCAT_CONFIG(usb_baudrate)
        
        DEFINE_CONCAT_CONFIG(eth_ip)
        DEFINE_CONCAT_CONFIG(eth_gateway)
        DEFINE_CONCAT_CONFIG(eth_subnet)
        DEFINE_CONCAT_CONFIG(eth_dns)
        
        DEFINE_CONCAT_CONFIG(ros2_namespace)
        DEFINE_CONCAT_CONFIG(ros2_nodename)
        DEFINE_CONCAT_CONFIG(ros2_readio_hz)

        DEFINE_CONCAT_CONFIG(wifi_ssid)
        DEFINE_CONCAT_CONFIG(wifi_pswd)

        DEFINE_CONCAT_CONFIG(can_rate)
        DEFINE_CONCAT_CONFIG(rs485_rate)

        config.concat("\n$board=ros2_multi_protocol");
        config.concat("\n$version=v1.0.0\n");
        return config;
    }


    DEFINE_GET_CONFIG_STR(transport_mode, CONFIG_DEFAULT_TRANSPORT_MODE)
    DEFINE_GET_CONFIG_STR(server_ip, CONFIG_DEFAULT_TRANSPORT_MODE)
    DEFINE_GET_CONFIG_INT(server_port, CONFIG_DEFAULT_SERVER_PORT)
    DEFINE_GET_CONFIG_INT(usb_baudrate, CONFIG_DEFAULT_USB_BAUD)

    DEFINE_GET_CONFIG_STR(eth_ip, CONFIG_DEFAULT_ETH_STATIC_IP)
    DEFINE_GET_CONFIG_STR(eth_gateway, CONFIG_DEFAULT_ETH_STATIC_GATEWAY)
    DEFINE_GET_CONFIG_STR(eth_subnet, CONFIG_DEFAULT_ETH_STATIC_SUBNET)
    DEFINE_GET_CONFIG_STR(eth_dns, CONFIG_DEFAULT_ETH_STATIC_DNS)

    DEFINE_GET_CONFIG_STR(wifi_ssid, CONFIG_DEFAULT_WIFI_SSID)
    DEFINE_GET_CONFIG_STR(wifi_pswd, CONFIG_DEFAULT_WIFI_PSWD)

    DEFINE_GET_CONFIG_STR(ros2_namespace, CONFIG_DEFAULT_ROS2_NAMESPACE)
    DEFINE_GET_CONFIG_STR(ros2_nodename, CONFIG_DEFAULT_ROS2_NODE_NAME)
    DEFINE_GET_CONFIG_INT(ros2_readio_hz, CONFIG_DEFAULT_ROS2_READ_IO_HZ)

    DEFINE_GET_CONFIG_INT(can_rate, CONFIG_DEFAULT_CAN_RATE)
    DEFINE_GET_CONFIG_INT(rs485_rate, CONFIG_DEFAULT_485_BAUDRATE)

    int8_t loop_config_uart(int c, char result[][32]);
    int8_t split_str(const char *line, char result[][32]);
    void deal_command(char key[32], char value[32]);

    RobotConfig(/* args */) = default;
    ~RobotConfig() = default;
};

extern RobotConfig SystemConfig;

#endif // __ROBOT_CONFIG_H__