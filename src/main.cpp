//#include <Arduino.h>
#include <Adafruit_BMP085.h>
#include <Adafruit_AHTX0.h>
#include <BH1750.h>
#include <Wire.h>
#include <WiFi.h>

#define LUX_ADDRESS 23
#define BMP180_ADDRESS 77
#define AHT20X_ADDRESS 38

#define LED_READY_PIN 13
#define LED_BUSY_PIN 19

#define ETH_LED_PIN 32
#define ETH_LED2_PIN 17

#define RAIN_HALL_PIN 35
#define WIND_HALL_PIN 25

#define BATTERY_PIN 33

#define batteryCalFactor .0011804
#define batteryLowVoltage 3.3
//#define HALL_ON 0

const char* ssid = "ssid";
const char* password = "password";

Adafruit_BMP085 bmp;
Adafruit_AHTX0 aht;
BH1750 bh_lux;
// Adafruit_Sensor *aht_humidity, *aht_temp;

byte bErrorBMP, bErrorLux, bErrorAHT;
byte bErrorAll = 0;

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  //randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}


void setup()
{
  pinMode(LED_READY_PIN, OUTPUT);
  pinMode(LED_BUSY_PIN, OUTPUT);
 // pinMode(ETH_LED_PIN, OUTPUT);
 // pinMode(ETH_LED2_PIN, OUTPUT);

   Serial.begin(9600);
  
 
WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }


  int nDevices = 0;
  // I2c Init
  Wire.begin(); // Open I2C communications as master

  // Serial init
  Serial.begin(115200);

  while (!Serial)
    delay(1000);

  Serial.println("\n\t\t\tBlacha Hardware Test\n####### **START SETUP\n");
  Serial.println("*I2C communication\tset->\tMaster");
  Serial.println("*I2C Scanning...");


  Wire.beginTransmission(35);
  bErrorLux = Wire.endTransmission();
  if (bErrorLux == 0)
    Serial.println("*Lux metter \t...\t0x23\t OK");
  else
  {
    Serial.println("*Lux metter \t...\t0x23\t Not Found");
    bErrorAll += 1;
  }
  sleep(1);


  Wire.beginTransmission(119);
  bErrorBMP = Wire.endTransmission();
  if (bErrorBMP == 0)
    Serial.println("*BMP180 sensor \t...\t0x77\t OK");
  else
  {
    Serial.println("*BMP180 sensor \t...\t0x77\t Not Found");
    bErrorAll += 1;
  }
  sleep(1);


  Wire.beginTransmission(56);
  bErrorAHT = Wire.endTransmission();
  if (bErrorAHT == 0)
    Serial.println("*AHT20X sensor \t...\t0x38\t OK");
  else
  {
    Serial.println("*AHT20X sensor \t...\t0x38\t Not Found");
    bErrorAll += 1;
  }
  sleep(1);


  // BMP180 Commpunication check
  if (bErrorBMP == 0)
    if (!bmp.begin())
    {
      Serial.println("*BMP180 sensor \tInit\t...\t Fail");
      bErrorBMP == 1;
      bErrorAll += 1;
    }
    else
      Serial.println("*BMP180 sensor \tInit\t...\t Ready");
  else
    Serial.println("*BMP180 sensor \tInit\t...\t Skipped");


  // AHT Communication check
  if (bErrorAHT == 0)
    if (!aht.begin())
    {
      Serial.println("*AHT20 sensor \tInit\t...\t Fail");
      bErrorAHT == 1;
      bErrorAll += 1;
    }
    else
      Serial.println("*AHT20 sensor \tInit\t...\t Ready");
  else
    Serial.println("*AHT20 sensor \tInit\t...\t Skipped");



  if (bErrorLux == 0)
  {
    Serial.println("*BH1750 sensor \tconf\t...\tONE_TIME_HIGH_RES_MODE");
    bh_lux.configure(BH1750::ONE_TIME_HIGH_RES_MODE);
    if (!bh_lux.begin())
    {
      Serial.println("*BH1750 sensor \tInit\t...\t Fail");
      bErrorLux == 1;
      bErrorAll += 1;
    }
    else
      Serial.println("*BH1750 sensor \tInit\t...\t Ready");
  }
  else
    Serial.println("*BH1750 sensor \tInit\t...\t Skipped");


  #ifdef HALL_ON
  Serial.println("=Hallotron Sensors: flag ON");
  Serial.print("*Rain sensor  \tInit\tPIN->\t");
  Serial.println(RAIN_HALL_PIN);
  Serial.print("*Wind sensor \tInit\tPIN->\t");
  Serial.println(WIND_HALL_PIN);
  #else
  Serial.println("Hallotron Sensors: flag OFF\tSkipped");
  #endif

  Serial.println("\n####### **END SETUP \n");


  if (bErrorAll == 0)
  {
    Serial.println("\t\t##POST PASSED WITHOUT ERRORS##");
  }
  else
  {
    Serial.print("\t\t##POST PASSED WITH ");
    Serial.print(bErrorAll);
    Serial.println(" ERRORS!##");
    //Serial.println("\t\t\tWait for restart...");
    //Serial.println("Restarting the ESP in 5 sec");
    // delay(5000);
    // ESP.restart();
  }
  Serial.println("\t\t\tSENSORS DATA");
}

void LedLoopFade(int pin, String name)
{
  int brightness = 0;
  int fadeAmount = 10;
  Serial.print("**Led '");
  Serial.print(name);
  Serial.print("' at GPIO:");
  Serial.println(pin);
  Serial.print(" -");
  for (int i = 0; i <= 51; i++)
  {
    analogWrite(pin, brightness);

    brightness = brightness + fadeAmount;

    if (brightness <= 0 || brightness >= 255)
    {
      Serial.print("@");
      fadeAmount = -fadeAmount;
    }
    else
      Serial.print(">");
    delay(30);
  }
  Serial.println(" done.");
  analogWrite(pin, 0);
}
void LedLoop(int pin)
{
  Serial.print("**Led at GPIO:");
  Serial.print(pin);
  Serial.print(" blinking: \n\t");
  digitalWrite(pin, HIGH);
  Serial.print("1 ");
  delay(300);
  digitalWrite(pin, LOW);
  Serial.print("0 ");
  delay(300);
  digitalWrite(pin, HIGH);
  Serial.print("1 ");
  delay(500);
  digitalWrite(pin, LOW);
  Serial.print("0 ");
  delay(500);
  digitalWrite(pin, HIGH);
  Serial.print("1 ");
  delay(1500);
  digitalWrite(pin, LOW);
  Serial.println("0 ");
  delay(300);
}
//==================================================================

void loop()
{
Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());


  if (bErrorAHT == 0)
  {
    sensors_event_t humidity, temp;
    aht.getEvent(&humidity, &temp); // populate temp and humidity objects with fresh data
    Serial.println("**AHT20X:");
    Serial.print("  Temperature: ");
    Serial.print(temp.temperature);
    Serial.println(" degrees C");
    Serial.print("  Humidity: ");
    Serial.print(humidity.relative_humidity);
    Serial.println("% rH\n");
  }
  else
    Serial.println("**AHT20X skipped");

 


  if (bErrorBMP == 0)
  {
    Serial.println("**BMP180:");
    //  Serial.print("  Temperature: ");
    //  Serial.print(bmp.readTemperature());
    //  Serial.println(" degrees C");
    Serial.print("  Preasure: ");
    Serial.print(bmp.readPressure());
    Serial.println(" Pa");
    Serial.print("  Preasure at sealevel: ");
    Serial.print(bmp.readSealevelPressure());
    Serial.println(" Pa\n");
    //  Serial.print("  Altitude = "); Serial.print(bmp.readAltitude()); Serial.println(" metters");
  }
  else
    Serial.println("**BMP120 skipped");

  if (bErrorLux == 0)
  {
    float lux = bh_lux.readLightLevel();
    Serial.println("**BH1750:");
    Serial.print("  Lux level: ");
    Serial.println(lux);
    Serial.println();
  }
  else
    Serial.println("**BH1750 skipped");

  delay(500);


int analogvolt =0;
Serial.println("**Battery: ");
for (int i = 0; i < 5; i++)
  {
    delay(500);
    analogvolt = analogRead(BATTERY_PIN);
    Serial.print("\t");
    Serial.print(analogvolt);
  }
Serial.println();
#ifdef HALL_ON
  int Rain_hall = 0;
  Serial.println("**RAIN HALL:");
  Serial.println("  Measure:");
  for (int i = 0; i < 5; i++)
  {
    Rain_hall = analogRead(RAIN_HALL_PIN);
    Serial.print("\t");
    Serial.print(Rain_hall);
  }
  Serial.println("\n");
#endif

#ifdef HALL_ON
  int Wind_hall = 0;
  Serial.println("**WIND HALL:");
  Serial.println("  Measure:");
  for (int i = 0; i < 5; i++)
  {
    Wind_hall = analogRead(WIND_HALL_PIN);
    Serial.print("\t");
    Serial.print(Wind_hall);
  }
  Serial.println("\n");
#endif


  // LEDs
  // LedLoop(LED_READY_PIN);
  LedLoopFade(LED_READY_PIN, "ReadyLed");
  LedLoopFade(LED_BUSY_PIN, "BusyLed");
 // LedLoopFade(ETH_LED_PIN, "EhernetLed_1");
 // LedLoopFade(ETH_LED2_PIN, "EhernetLed_2");
  // LedLoop(LED_BUSY_PIN);
  // LedLoop(ETH_LED_PIN);
  // LedLoop(ETH_LED2_PIN);

   
}