
//include relevant libraries
#include <SHT1x.h>
#include "LiquidCrystal.h"

//initialise library with numbers of interface pins
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

//Globals for SHT10 sensor
#define dataPin 3        // what pin SHT10 data line is connected to
#define clockPin 2      // what pin SHT10 clock line is connected to
SHT1x sht1x(dataPin, clockPin);     // instantiate SHT10 sensor

//humidity control points
#define HSETPOINT 50
#define HDEADBAND 2.5
//Temperature control points
#define TSETPOINT 23
#define TDEADBAND 0.5

//make code easier to read
#define HUMIDIFIER 13   // name it HUMIDIFIER instead of pin 13
#define HEATER 12      // name it HEATER instead of pin 12
#define OFF false      // relay is NOT backwards. low/0/false = OFF
#define ON true     // relay is NOT backwards. high/1/true = ON

void setup() {
//tell arduino we'll use the HUMIDIFIER pin as an output & set it to OFF
  pinMode(HUMIDIFIER, OUTPUT);
  digitalWrite(HUMIDIFIER, OFF);
  pinMode(HEATER, OUTPUT);
  digitalWrite(HEATER, OFF);

//initialise LCD and tell library its size (columns, rows)
  lcd.begin(16, 2);

//start the code for SHT sensor ?????

//setup stuff that never changes on screen
  lcd.clear();          //should be clear anyway so maybe unnecessary
}

//routine to be executed continuously
void loop() {
  delay(5000);      //5sec delay
  
//read temp and humidity into floating point variables
  float t = sht1x.readTemperatureC();
  float h = sht1x.readHumidity();

//checking if we got back numbers. If not, print error message
  if (isnan(t) || isnan(h)) {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Failed to read");
    lcd.setCursor(0,1);
    lcd.print("SHT sensor");
    return;      //goes back up and waits another 5sec to retry sensor.
  }
  else
  {
    lcd.clear();
    lcd.setCursor(0,0);   //go to character zero, line zero
    lcd.print("Temp: ");
    lcd.setCursor(0,1);
    lcd.print("RH: ");

    lcd.setCursor(10,0);  //move cursor to after the word Temp:
    lcd.print(t,2);      //print temp to 1 decimal place
    lcd.print("C");
    lcd.setCursor(10,1);
    lcd.print(h,2);
    lcd.print("%");
  }

//now the control logic
  if(digitalRead(HUMIDIFIER) == ON)  //read humidifier pin to see if it's on
  {
    if( h > HSETPOINT + HDEADBAND)
    {
      digitalWrite(HUMIDIFIER, OFF);  //upper limit reached so turn off humidifier
    }
  }
  else
  {
    if( h < HSETPOINT - HDEADBAND)
    {
      digitalWrite(HUMIDIFIER, ON);  //lower limit reached so turn on humidifier
    }
  }
  {
    if(digitalRead(HEATER) == ON)  //read heater pin to see if it's on
    {
      if( t > TSETPOINT + TDEADBAND)
      {
        digitalWrite(HEATER, OFF);  //upper limit reached so turn off heater
      }
    }
    else
    {
      if( t < TSETPOINT - TDEADBAND)
      {
        digitalWrite(HEATER, ON);   //lower limit reached so turn on heater
      }
    }
  }
}
