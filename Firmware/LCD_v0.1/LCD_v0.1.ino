/*
 Read all data after interrupt
 by Mischianti Renzo <http://www.mischianti.org>

 https://www.mischianti.org/2019/01/02/pcf8574-i2c-digital-i-o-expander-fast-easy-usage/
*/

#include "Arduino.h"
#include "PCF8574.h"

// For arduino uno only pin 1 and 2 are interrupted
#define INTERRUPTED_D1_PIN 2
#define INTERRUPTED_D2_PIN 3
#define INTERRUPTED_D3_PIN A0

// Function interrupt
void keyChangedOnPCF8574_D1();
void keyChangedOnPCF8574_D2();
void keyChangedOnPCF8574_D3();

void initPCF8574(PCF8574* pcf8574, uint8_t mode){
  for(uint8_t pin = 0; pin < 8; pin++){
    pcf8574->pinMode(pin, mode);
  }
}

// Set i2c address
PCF8574 d1_InPcf8574(0x20, INTERRUPTED_D1_PIN, keyChangedOnPCF8574_D1);
PCF8574 d2_InPcf8574(0x21, INTERRUPTED_D2_PIN, keyChangedOnPCF8574_D2);
PCF8574 d3_InPcf8574(0x22, INTERRUPTED_D3_PIN, keyChangedOnPCF8574_D3);

PCF8574 d4_InPcf8574(0x24);
PCF8574 d5_InPcf8574(0x25);
PCF8574 d6_InPcf8574(0x26);
unsigned long timeElapsed;
void setup()
{
  //pinMode(_pin, INPUT_PULLUP);
	Serial.begin(115200);
	delay(1000);

  initPCF8574(&d1_InPcf8574, INPUT);
  initPCF8574(&d2_InPcf8574, INPUT);
  initPCF8574(&d3_InPcf8574, INPUT);

  initPCF8574(&d4_InPcf8574, OUTPUT);
  initPCF8574(&d5_InPcf8574, OUTPUT);
  initPCF8574(&d6_InPcf8574, OUTPUT);
  
	Serial.print("Init pcf8574...");
	if (d1_InPcf8574.begin() && d2_InPcf8574.begin() && d3_InPcf8574.begin() && d4_InPcf8574.begin() && d5_InPcf8574.begin() && d6_InPcf8574.begin()){
		Serial.println("OK");
	}else{
		Serial.println("KO");
	}


	Serial.println("START");

	timeElapsed = millis();

  pinMode(9, OUTPUT);
  digitalWrite(9, HIGH);
}

bool keyChanged_d1 = true;
bool keyChanged_d2 = true;
bool keyChanged_d3 = true;
void loop(){
  if(keyChanged_d1){
      PCF8574::DigitalInput d1 = d1_InPcf8574.digitalReadAll();
      InversSign(&d1);
      d4_InPcf8574.digitalWriteAll(d1);
	    printStatePins(&d1, 1);
      keyChanged_d1 = false;
  }

  if(keyChanged_d2){
      PCF8574::DigitalInput d2 = d2_InPcf8574.digitalReadAll();
      InversSign(&d2);
      d5_InPcf8574.digitalWriteAll(d2);
	    printStatePins(&d2, 2);
      keyChanged_d2 = false;
  }

  if(keyChanged_d3){
      PCF8574::DigitalInput d3 = d3_InPcf8574.digitalReadAll();
      InversSign(&d3);
      d6_InPcf8574.digitalWriteAll(d3);
	    printStatePins(&d3, 3);
      keyChanged_d3 = false;
  }

  if(digitalRead(INTERRUPTED_D3_PIN) == false){
    keyChangedOnPCF8574_D3();
  }

}

void printStatePins(PCF8574::DigitalInput* di, uint8_t PCF_D){
    Serial.print("READ VALUE FROM PCF-");
    Serial.print(PCF_D);
    Serial.print(": ");
		Serial.print(di->p0);
		Serial.print(" - ");
		Serial.print(di->p1);
		Serial.print(" - ");
		Serial.print(di->p2);
		Serial.print(" - ");
		Serial.print(di->p3);
    Serial.print(" - ");
		Serial.print(di->p4);
    Serial.print(" - ");
		Serial.print(di->p5);
    Serial.print(" - ");
		Serial.print(di->p6);
    Serial.print(" - ");
		Serial.println(di->p7);
}

void InversSign(PCF8574::DigitalInput* di){
  di->p0 = !di->p0;
  di->p1 = !di->p1;
  di->p2 = !di->p2;
  di->p3 = !di->p3;
  di->p4 = !di->p4;
  di->p5 = !di->p5;
  di->p6 = !di->p6;
  di->p7 = !di->p7;
}

void keyChangedOnPCF8574_D1(){
  Serial.println("D1");
  keyChanged_d1 = true;
}

void keyChangedOnPCF8574_D2(){
  Serial.println("D2");
  keyChanged_d2 = true;
}

void keyChangedOnPCF8574_D3(){
  Serial.println("D3");
  keyChanged_d3 = true;
}
