#ifndef DEFAULT_MODULES_H
#define DEFAULT_MODULES_H


class Default_modules {
public:
    static const int NONE = 0;
    static const int DEAD_MAN_SWITCH = 1;
    static const int NEOPIXELS = 2;
    static const int FASTLED = 3;
    static const int SERIAL_CONTROL = 4;
    static const int JOYSTICK_CONTROL = 5;
    static const int FORCE_CONTROL = 6;
    static const int POWER_LIMIT = 7;
    static const int CURRENT_MONITOR = 8;
    static const int VOLTAGE_MONITOR = 9;
    static const int TEMPERATURE_MONITOR = 10;
    static const int DIRECT_POWER_MODULATION = 11;
    static const int EXPONENTIAL_POWER_MODULATION = 12;
    static const int WIFI_CONNECTION = 13;
    static const int WIFI_SIMPLE_CONTROL = 14;
    static const int BT_CONNECTION = 15;
    static const int WEB_UPDATE = 16;
    static const int SPIFFS_DRIVELOG = 17;
    static const int VESC_CONTROLLER = 18;
    static const int PWM_STEERING = 19;
    static const int PWM_CONTROLLER = 20;
    static const int WEBSOCKET_CONTROL = 21;
    static const int CONTROL_PRIORITY = 22;
    static const int NTP_TIMESERVICE = 23;
    static const int ARDUINO_OTA = 24;
    static const int WEBSOCKET_CONNECTION = 25;
    static const int WEBSOCKET_DRIVELOG = 26;
    static const int MQTT_CLIENT = 27;
    static const int ESP32_DIGITAL_LED = 28;
    static const int ANALOG_SENSOR = 29;
    static const int BOOLEAN_SENSOR = 30;
    static const int SERIAL_LOG = 31;
    static const int SPIFFS_LOG = 32;
    static const int WEBSOCKET_SERVER_LOG = 33;

};

#endif
