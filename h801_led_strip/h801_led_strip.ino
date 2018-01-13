#include <Homie.h>
#include <ArduinoOTA.h>

#define PIN_RED   15
#define PIN_GREEN 13
#define PIN_BLUE  12
#define PIN_W1    14
#define PIN_W2    4
#define LED_RED   5
#define LED_GREEN 1

#define BRAND "rgb-strip"
#define FW_NAME "rgb-strip"
#define FW_VERSION "1.0.1"

/**
 * Arduino IDE instructions
 * 
 * select "generic ESP8266 Module"
 * 80Mhz CPU, 40Mhz Flash
 * Upload Speed 921600
 * 1M (64K SPIFFS)
 * Flash Mode QIO
 * 
 * 
 * to control the Colors, send the String R,G,B,W1,W2 via MQTT
 * the values for R G B W1 and W2 are from 0 to 1023
 * the topic will be "homie/DEVICEID/strip/color/set
 */

HomieNode lightNode("strip", "rgbww");


String getValue(String data, char separator, int index) {
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
        found++;
        strIndex[0] = strIndex[1] + 1;
        strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }

  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

bool nodeInputHandler(const HomieRange& range, const String& value) {
  String r_str = getValue(value, ',', 0);
  String g_str = getValue(value, ',', 1);
  String b_str = getValue(value, ',', 2);
  String w1_str = getValue(value, ',', 3);
  String w2_str = getValue(value, ',', 4);

  if(r_str == "" || g_str == "" || b_str == "" || w1_str == "" || w2_str == "") {
    return false; 
  }

  int r = r_str.toInt();
  int g = g_str.toInt();
  int b = b_str.toInt();
  int w1 = w1_str.toInt();
  int w2 = w2_str.toInt();

  analogWrite(PIN_RED, r);
  analogWrite(PIN_GREEN, g);
  analogWrite(PIN_BLUE, b);
  analogWrite(PIN_W1, w1);
  analogWrite(PIN_W2, w2);
  
  lightNode.setProperty("color").send(value);
  
  return true;
}



void setup() {
  Serial.begin(115200);
  Serial << endl << endl;
  pinMode(PIN_RED, OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_BLUE, OUTPUT);
  pinMode(PIN_W1, OUTPUT);
  pinMode(PIN_W2, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  
  Homie.setLedPin(LED_RED, HIGH);
  Homie_setFirmware(FW_NAME, FW_VERSION);
  Homie_setBrand(BRAND);

  lightNode.advertise("color").settable(nodeInputHandler);

  Homie.setup();
  Homie.getLogger() << "started" << endl;
  ArduinoOTA.setHostname(Homie.getConfiguration().deviceId);
  ArduinoOTA.begin();
}

void loop() {
  Homie.loop();
  ArduinoOTA.handle();
}

