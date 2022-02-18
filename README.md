# esp8266-homekit-watchdog
ESP8266 HomeKit Watchdog

Uses [Arduino-HomeKit-ESP8266](https://github.com/Mixiaoxiao/Arduino-HomeKit-ESP8266) library.

This is an Arduino sketch that allows you to create HomeKit-integrated watchdog, a device that triggers a HomeKit programmable switch once powered on and then every 5 minutes. Very suitable if you, for example want to keep some device running based on another device power state. E.g. you may turn on your HomeKit powered speakers on once the TV (and it's USB port) is turned on. Very practical when combined with [esp8266-homekit-infrared-remote](https://github.com/grmelacz/esp8266-homekit-infrared-remote).

This sketch has a programmable stateless switch implemented. It is triggered once the device boots up and then every 5 minutes.

## How to get it up & running

1. Obtain some ESP8266-based board, e.g. Wemos D1.
2. Compile & upload the sketch and connect it the USB port of the device that you need to monitor.
3. [Pair the board](https://github.com/Mixiaoxiao/Arduino-HomeKit-ESP8266#usage) with your Home and set up the automation for a single press as needed.
4. You've got a HomeKit watchdog!
