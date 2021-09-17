#include <SoftwareSerial.h>
#include <Wire.h>
SoftwareSerial nodemcu(2,3);
#include <Adafruit_INA219.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>
long int data; 

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);
Adafruit_INA219 ina219;

unsigned long previousMillis = 0;
unsigned long interval = 100;
float shuntvoltage = 0;
float busvoltage = 0;
float current_mA = 0;
float loadvoltage = 0;
float energy = 0;

float sdata1 = 0; 
float sdata2 = 0; 
float sdata3 = 0;
float sdata4 = 0; 

String cdata; 

void setup()
{
Serial.begin(9600); 
nodemcu.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  ina219.begin();
}


void loop()
{
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval)
  {
    previousMillis = currentMillis;
    ina219values();
    displaydata();
  }
if(nodemcu.available() == 0 )
{
    sdata1 = loadvoltage;
    sdata2 = current_mA;
    sdata3 = loadvoltage * current_mA;
    sdata4 = energy;

    
   cdata = cdata + sdata1+","+sdata2+","+sdata3+","+sdata4; 
   Serial.println(cdata); 
   nodemcu.println(cdata);
   delay(1000); 
   cdata = ""; 
}
}


void displaydata() {
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println(loadvoltage);
  display.setCursor(31, 0);
  display.println("V");
  display.setCursor(62, 0);  
  display.setCursor(75, 0);
  display.println(current_mA);
  display.setCursor(110, 0);
  display.println("mA");
  display.setCursor(0, 6);
  display.println("--------------------"); 

  
    display.setCursor(101, 14);
    display.println("A");
    display.setCursor(108, 17);
    display.println("b");
    display.setCursor(114, 20);
    display.println("i");
    display.setCursor(120, 23);
    display.println("d");


    
  display.setCursor(0, 13);
  display.println(loadvoltage * current_mA);
  display.setCursor(57, 13);
  display.println("mW");
  display.setCursor(0, 23);
  display.println(energy);
  display.setCursor(57, 23);
  display.println("mWh");
  display.display();
}

void ina219values() {
  shuntvoltage = ina219.getShuntVoltage_mV();
  busvoltage = ina219.getBusVoltage_V();
  current_mA = ina219.getCurrent_mA();
  loadvoltage = busvoltage + (shuntvoltage / 1000);
  energy = energy + loadvoltage * current_mA / 3600;
}
