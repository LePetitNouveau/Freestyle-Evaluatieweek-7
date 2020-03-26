//blynk
#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

//Servo
#include "esp32-hal-ledc.h"

//OLED
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);


char auth[] = "PrWk_OdGEUPnAvgScTRLXzkk-wPJiw9j";
char ssid[] = "bbox2-hadja2";
char pass[] = "turlututu1";

//------------ licht sensor ------------
float lightValA;
float lightValB;
int lightValC;
int AanUit = 0;

BLYNK_READ(V1) {
  lightValA = analogRead(34);
  lightValB = lightValA / 4095;
  lightValC = lightValB * 100;
  Blynk.virtualWrite(V1, lightValC);
  Blynk.syncVirtual(V1);
  delay(10);
}

//----------- licht voorwaarde -----------
int LichtVw;

BLYNK_WRITE(V2) {
  LichtVw = param.asInt();
  Serial.println(LichtVw);
}

//---------- cyclussen te maken ----------
int Cyclus;
int CyclusTD;
BLYNK_WRITE(V3) {
  Cyclus = param.asInt();
  CyclusTD = Cyclus;
  Serial.println(Cyclus);
}








void setup() {
  Serial.begin(9600);
  pinMode(2, OUTPUT);

  Blynk.begin(auth, ssid, pass, "server.wyns.it", 8081);

  ledcSetup(1, 50, 16);
  ledcAttachPin(2, 1);


  Serial.begin(115200);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

  delay(2000);

}



void loop() {
  Blynk.run();

  OLED();

  if (CyclusTD != 0) {
    for (int i = 100 ; i < 8000 ; i = i + 100) {
      delay(25);
      ledcWrite(1, i);
    }
    lightValA = analogRead(34);
    lightValB = lightValA / 4095;
    lightValC = lightValB * 100;
    Blynk.virtualWrite(V1, lightValC);
    OLED();
    delay(300);
    if (LichtVw > lightValC) {
    AanUit = 1;
  }

  else {
    AanUit = 0;
  }
    
    for (int i = 8000 ; i > 1000 ; i = i - 100) {
      delay(25);
      ledcWrite(1, i);
    }
    CyclusTD = CyclusTD - 1;
  }

  OLED();
  delay(300);
if (LichtVw > lightValC) {
    AanUit = 1;
  }

  else {
    AanUit = 0;
  }

}


void OLED() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 5);
  display.print("Lichthelderheid: ");
  display.print(lightValC);
  display.println("%");

  display.setCursor(0, 20);
  display.print("Voorwaarde: ");
  display.print(LichtVw);
  display.println("%");

  display.setCursor(0, 35);
  display.print("Lichtstand: ");
  if (AanUit == 1) {
    display.print("Donker");
  }
  else {
    display.print("Helder");
  }

  display.setCursor(0, 50);
  display.print("Cyclus in wacht: ");
  display.print(CyclusTD);

  display.display();
}
