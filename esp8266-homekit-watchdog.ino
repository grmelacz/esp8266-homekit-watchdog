/*
 * ESP8266 HomeKit Watchdog
 * By Jan Grmela <grmela@gmail.com> in 2022
 * License: GPL v3 or newer
 * Version: 1.0
 * 
 * Based on Arduino-HomeKit-ESP8266 examples by Mixiaoxiao (Wang Bin)
 * 
 * Tested successfully on a $2 WEMOS D1 mini clone
 * with Arduino 1.8.20 in 2/2022
 * 
 * == Function ==
 * - a stateless programmable switch that is triggered once the board is powered on 
 * and then every 2 minutes.
 *
 */
#include <Arduino.h>
#include <arduino_homekit_server.h>
#include "wifi_info.h"

/* USER CONFIG */
// LED to blink once transmitted (2 = internal LED)
#define LED_PIN 2
// confirmation LED blink & connected clients log interval
#define BLINK_INTERVAL 1000*10 // 10 seconds
// how long should the stateful switch stay enabled
#define WATCHDOG_INTERVAL 1000*120 // 2 mins

/* GLOBAL VARS */
#define LOG_D(fmt, ...)   printf_P(PSTR(fmt "\n") , ##__VA_ARGS__);
extern "C" homekit_server_config_t config;
extern "C" homekit_characteristic_t cha_programmable_switch_event;
static uint32_t next_heap_millis = 0;
static uint32_t next_bark_millis = 0;
#define HOMEKIT_PROGRAMMABLE_SWITCH_EVENT_SINGLE_PRESS   0

void setup() {
  Serial.begin(115200);
  wifi_connect(); // in wifi_info.h
  //homekit_storage_reset(); // To remove the previous HomeKit pairing storage when you first run this new HomeKit example
  my_homekit_setup();


  // Set the LED default state to off
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);
}

void loop() {
  my_homekit_loop();
  delay(10);
}

// Called when the value is read by iOS Home APP
homekit_value_t cha_programmable_switch_event_getter() {
  // Should always return "null" for reading, see HAP section 9.75
  return HOMEKIT_NULL_CPP();
}

// Trigger the switch single press event
void bark() {
  cha_programmable_switch_event.value.uint8_value = HOMEKIT_PROGRAMMABLE_SWITCH_EVENT_SINGLE_PRESS;
  homekit_characteristic_notify(&cha_programmable_switch_event,
  cha_programmable_switch_event.value);
}

void my_homekit_setup() {
  // Create getter for the switch & configure the HomeKit
  cha_programmable_switch_event.getter = cha_programmable_switch_event_getter;
  arduino_homekit_setup(&config);
}

void my_homekit_loop() {
  arduino_homekit_loop();
  const uint32_t t = millis();

  // Periodic LED blink & heap info
  if (t > next_heap_millis) {
    blink_led();
    next_heap_millis = t + BLINK_INTERVAL;
    LOG_D("Free heap: %d, HomeKit clients: %d", ESP.getFreeHeap(), arduino_homekit_connected_clients_count());
  }

  // Watchdog every X minutes triggers the switch
  if(t > next_bark_millis) {
    // Trigger the switch
    bark();
    next_bark_millis = t + WATCHDOG_INTERVAL;
    LOG_D("Watchdog triggered. Next trigger scheduled to %d seconds device time.", next_bark_millis/1000);
  }
}

void blink_led(void) {
  digitalWrite(LED_PIN, LOW);
  delay(100);
  digitalWrite(LED_PIN, HIGH);
}
