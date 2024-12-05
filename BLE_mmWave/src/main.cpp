#include <Arduino.h>
#include "Seeed_Arduino_mmWave.h"
#include <Adafruit_NeoPixel.h>
#include <hp_BH1750.h>

#define SUCCESSFUL 1
#define FAILED 0

int sensor_status;
const int pixelPin = D1;

// If the board is an ESP32, include the HardwareSerial library and create a
// HardwareSerial object for the mmWave serial communication
#ifdef ESP32
#  include <HardwareSerial.h>
HardwareSerial mmWaveSerial(0);
#else
// Otherwise, define mmWaveSerial as Serial1
#  define mmWaveSerial Serial1
#endif

SEEED_MR60BHA2 mmWave;
hp_BH1750 BH1750;

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(1, pixelPin, NEO_GRB + NEO_KHZ800);

/*这里只可以float?????*/

typedef struct mmWave_Variable{
  float total_phase;
  float breath_phase;
  float heart_phase;
  float breath_rate;
  float heart_rate;
  float distance;
}mmWave_Variable;

mmWave_Variable mmWave_A;

void setup() {
  Serial.begin(115200);

  Driver_All();
}

void loop() {
  mmWave_Sensor_Driver();
  sensor_determine();
  LED_Sensor_Driver();
  delay(100);
}

void Driver_All(){
  pixels.begin();
  pixels.clear();
  pixels.show();

  mmWave.begin(&mmWaveSerial);

  BH1750.begin(BH1750_TO_GROUND);
}



int mmWave_Sensor_Driver(){
  if (mmWave.update(100)) {
    if (mmWave.getHeartBreathPhases(mmWave_A.total_phase, mmWave_A.breath_phase, mmWave_A.heart_phase)) {
      Serial.printf("total_phase: %.2f\t", mmWave_A.total_phase);
      Serial.printf("breath_phase: %.2f\t", mmWave_A.breath_phase);
      Serial.printf("heart_phase: %.2f\n", mmWave_A.heart_phase);
    }
    if (mmWave.getBreathRate(mmWave_A.breath_rate) !=0 && mmWave.getHeartRate(mmWave_A.heart_rate) !=0 && mmWave.getDistance(mmWave_A.distance) !=0) {
      Serial.printf("breath_rate: %.2f\n", mmWave_A.breath_rate);
      Serial.printf("heart_rate: %.2f\n", mmWave_A.heart_rate);
      Serial.printf("distance: %.2f\n", mmWave_A.distance);
    }
    return SUCCESSFUL;
  }else{
    return FAILED;
  }
  BHT1750_Sensor_Driver();
}


int BHT1750_Sensor_Driver(){
  BH1750.start();
    float lux = BH1750.getLux();
    if(lux){
    Serial.println("the light intensity is ");
    Serial.println(lux);
    Serial.println("");
    return 1;
  }else{
    return 0;
  }
}


int sensor_determine(){
  int TAG_A = BHT1750_Sensor_Driver();
  int TAG_B = mmWave_Sensor_Driver();
  if(TAG_A == 1 && TAG_B == 1){
    return SUCCESSFUL;
  }else{
    return FAILED;
  }
}


void LED_Sensor_Driver(){
  switch(sensor_determine())
  {
    case SUCCESSFUL:
      pixels.setPixelColor(175, pixels.Color(0, 255, 0));
      pixels.show();
      break;
    case FAILED:
      pixels.setPixelColor(175,pixels.Color(255,0,0));
      pixels.show();
      break;
  }
}





