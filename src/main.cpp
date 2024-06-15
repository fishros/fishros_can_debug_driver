#include <Arduino.h>

#include <ESP32-TWAI-CAN.hpp>
#include <OneButton.h>
#include "robot_config.h"
#include "can_debug/can_debug.h"

#include "soc/rtc_wdt.h" //设置看门狗用
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"

uint16_t led_period = 100;

void led_wdt_task(void *param)
{
  rtc_wdt_protect_off(); // 看门狗写保护关闭 关闭后可以喂狗
  rtc_wdt_enable();      // 启用看门狗
  pinMode(12, OUTPUT);
  digitalWrite(12, HIGH);
  while (true)
  {
    rtc_wdt_feed(); // 喂狗
    delay(20);
    if (led_period)
    {
      RUN_EVERY(led_period, {
        digitalWrite(12, !digitalRead(12));
      });
    }
  }
}

OneButton button(15, true);

void buttonDoubleClick()
{
  led_period = 1000;
  delay(1000);
  led_period = 100;
  if (SystemConfig.get_config_transport_mode() == CONFIG_TRANSPORT_MODE_USB)
  {
    SystemConfig.config("transport_mode", CONFIG_TRANSPORT_MODE_ETH_STATIC);
  }
  else if (SystemConfig.get_config_transport_mode() == CONFIG_TRANSPORT_MODE_ETH_STATIC)
  {
    SystemConfig.config("transport_mode", CONFIG_TRANSPORT_MODE_ETH_DHCP);
  }
  else if (SystemConfig.get_config_transport_mode() == CONFIG_TRANSPORT_MODE_ETH_DHCP)
  {
    SystemConfig.config("transport_mode", CONFIG_TRANSPORT_MODE_WIFI_STA);
  }
  else if (SystemConfig.get_config_transport_mode() == CONFIG_TRANSPORT_MODE_WIFI_STA)
  {
    SystemConfig.config("transport_mode", CONFIG_TRANSPORT_MODE_USB);
  }
  delay(10);
  esp_restart();
}

void setup()
{
  Serial.setRxBufferSize(4096);
  Serial.setTxBufferSize(4096);
  Serial.begin(1500000);
  botlog_set_target(Serial);
  SystemConfig.init();
  button.attachDoubleClick(buttonDoubleClick);
  // =================ETH CLOCK ENABLE==================
  pinMode(2, OUTPUT);
  digitalWrite(2, LOW);
  digitalWrite(2, HIGH);
  delay(10);
  
  ESP32Can.setPins(4, 5);
  ESP32Can.setSpeed(ESP32Can.convertSpeed(SystemConfig.get_config_can_rate()));
  ESP32Can.setRxQueueSize(5);
  ESP32Can.setTxQueueSize(5);
  ESP32Can.begin();
  
  xTaskCreatePinnedToCore(led_wdt_task, "led_wdt_task", 1024, NULL, 1, NULL, 0);
  xTaskCreatePinnedToCore(can_debug_task, "can_debug_task", 40960, NULL, 2, NULL, 1);
}

void loop()
{
  delay(20);
  button.tick();
}