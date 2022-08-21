
//===========================================
//WiFi connection
//===========================================
char ssid[] = "ssid"; // WiFi Router ssid
char pass[] = "password"; // WiFi Router password

//===========================================
//MQTT broker connection
//===========================================
//const char* mqttServer = "test.mosquitto.org";
const char* mqttServer = "192.168.5.74";
const int mqttPort = 1883;
const char* mqttUser = "username";
const char* mqttPassword = "password";
const char mainTopic[20] = "MainTopic/";


//===========================================
//Metric or Imperial measurements
//===========================================
#define METRIC

//===========================================
//BME280 altitude offsets (set by user)
//===========================================
#define ALTITUDE_OFFSET_IMPERIAL 5.58
#define ALTITUDE_OFFSET_METRIC 142.6


//===========================================
//BH1750 Enable
//===========================================
#define BH1750Enable


//===========================================
//General defines
//===========================================
#define RSSI_INVALID -9999


//===========================================
//Battery calibration
//===========================================
//batteryCalFactor = measured battery voltage/ADC reading
#define batteryCalFactor .0011804
#define batteryLowVoltage 3.3


//===========================================
//Timezone information
//===========================================
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = -7 * 3600;
const int   daylightOffset_sec = 3600;

//========================= Enable Blynk, MQTT or Thingspeak ===================================
//const String App = "BLYNK";         //  alternative is line below
//const String App = "Thingspeak"; //  alternative is line above
//const String App = "MQTT";         //  alternative is line below




//===========================================
//Set how often to wake and read sensors
//===========================================
//const int UpdateIntervalSeconds = 15 * 60;  //Sleep timer (900s) for my normal operation
const int UpdateIntervalSeconds = 5 * 60;  //Sleep timer (60s) testing
