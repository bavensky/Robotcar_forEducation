#include <LCD5110_Basic.h>    // เรียกใช้งานไลบรารี่จอแสดงผล 5110
#include <avr/pgmspace.h>
// This program requires a Nokia 5110 LCD module.
// correct voltage to the module.
//      SCK  - Pin 10
//      MOSI - Pin 11
//      DC   - Pin 12
//      RST  - Pin 13
//      CS   - Pin 8
//
LCD5110 myGLCD(10,11,12,13,8);
extern uint8_t arduino_logo[];  //  logo arduino
extern uint8_t SmallFont[];
extern uint8_t MediumNumbers[];
extern uint8_t BigNumbers[];

#define SENSOR_1  A2  
#define SENSOR_2  A3
#define SWITCH    A5

int motorA_a = 9;  
int motorA_b = 6; 
int motorB_a = 5;   
int motorB_b = 3;  

unsigned int mode = 0;
int function = 0;
int switch_all = 0; 
char key;

int sensorValue1 = 0;    
int sensorValue2 = 0;  
int rang1 = 0;
int rang2 = 0;
int outputValue = 0;       

void setup() {

  Serial.begin(9600);
  myGLCD.InitLCD();
  
  pinMode(SWITCH,  INPUT);
  pinMode(SENSOR_1,  INPUT);
  pinMode(SENSOR_2,  INPUT);
    
  pinMode(motorA_a, OUTPUT);        
  pinMode(motorA_b, OUTPUT);      
  pinMode(motorB_a, OUTPUT);       
  pinMode(motorB_b, OUTPUT);
  
  myGLCD.drawBitmap(0, 0, arduino_logo, 84, 48);  
// delay(2000);
// myGLCD.clrScr();
}

void loop() {
/************  Drive Motor  ****************************
Motor Right
  motorA_a  =  1  Go Forward
  motorA_b  =  1  Go Reverse
Motor Left  
  motorB_a  =  1  Go Reverse
  motorB_b  =  1  Go Forward
********************************************************/

  read_switch();
  switch(key)  {
    case 'E' :
      mode += 1;
      myGLCD.clrScr();
    break;
  }
  if(mode >= 3)  {
    mode = 0;  
  }
  while(mode == 0)  {
    myGLCD.drawBitmap(0, 0, arduino_logo, 84, 48);  
    read_sensor() ;
    
    read_switch();
    switch(key)  {
    case 'E' :
      mode += 1;
      myGLCD.clrScr();
    break;
    case 'C' :
      mode -= 1;
      myGLCD.clrScr();
    break;
   }
    
  }
    
  while(mode == 1)  {
    read_switch();
    switch(key)  {
    case 'E' :
      mode += 1;
      myGLCD.clrScr();
    break;
    case 'C' :
      mode -= 1;
      myGLCD.clrScr();
    break;
    case 'P' :
      rang1 += 1;
    break;
    case 'M' :
      rang1 -= 1;
    break;
   }
   
    sensorValue1 = analogRead(SENSOR_1);
    sensorValue2 = analogRead(SENSOR_2);
    
    myGLCD.setFont(SmallFont);
    myGLCD.print("Line Follow !", LEFT, 0);
    myGLCD.print("Sensor    ", LEFT, 8);
    myGLCD.print(" Left     ", LEFT, 16);
    myGLCD.print("Set       ", LEFT, 32);
    myGLCD.print(" Rang     ", LEFT, 40);
    myGLCD.setFont(MediumNumbers);
    myGLCD.printNumI(sensorValue1, RIGHT, 8);
    myGLCD.printNumI(rang1, RIGHT, 32);
    myGLCD.printNumI(switch_all, RIGHT, 40);
  }
  
   while(mode == 2)  {
     read_switch();
    switch(key)  {
    case 'E' :
      mode += 1;
      myGLCD.clrScr();
    break;
    case 'C' :
      mode -= 1;
      myGLCD.clrScr();
    break;
    case 'P' :
      rang2 += 1;
    break;
    case 'M' :
      rang2 -= 1;
    break;
   }
   
    sensorValue1 = analogRead(SENSOR_1);
    sensorValue2 = analogRead(SENSOR_2);
    
    myGLCD.setFont(SmallFont);
    myGLCD.print("Line Follow !", LEFT, 0);
    myGLCD.print("Sensor    ", LEFT, 8);
    myGLCD.print(" Right    ", LEFT, 16);
    myGLCD.print("Set       ", LEFT, 32);
    myGLCD.print(" Rang     ", LEFT, 40);
    myGLCD.setFont(MediumNumbers);
    myGLCD.printNumI(sensorValue2, RIGHT, 8);
    myGLCD.printNumI(rang2, RIGHT, 32);
    myGLCD.printNumI(switch_all, RIGHT, 40);
   }
}

void read_switch()  {
  switch_all  =  analogRead(SWITCH);
  if(switch_all > 850)  {
    delay(200);
    key = 'X';
  }
  if(switch_all > 720 && switch_all < 800)  {
    delay(200);
    key = 'M';
  }
  if(switch_all > 450  &&  switch_all < 550)  {
    delay(200);
    key = 'S';
  }
  if(switch_all > 300  &&  switch_all < 400)  {
    delay(200);
    key = 'U';
  }
  if(switch_all < 200)  {
    delay(200);
    key = 'D';
  }
}

void read_sensor()  {
  sensorValue1 = analogRead(SENSOR_1);
  sensorValue2 = analogRead(SENSOR_2);
  
  if(sensorValue1 < rang1  && sensorValue2 < rang2)  {
    analogWrite(motorA_a,  128);
    analogWrite(motorA_b,  0);
    analogWrite(motorB_a,  0);
    analogWrite(motorB_b,  128);
  }
  
  if(sensorValue1 < rang1  &&  sensorValue2  >  rang2)  {
    analogWrite(motorA_a,  0);
    analogWrite(motorA_b,  0);
    analogWrite(motorB_a,  0);
    analogWrite(motorB_b,  128); 
  }
  
  if(sensorValue1 > rang1  &&  sensorValue2  <  rang2)  {
    delay(200);
    analogWrite(motorA_a,  128);
    analogWrite(motorA_b,  0);
    analogWrite(motorB_a,  0);
    analogWrite(motorB_b,  0); 
  }
  if(sensorValue1 > rang1 && sensorValue2 > rang2)  {
    analogWrite(motorA_a,  1);
    analogWrite(motorA_b,  1);
    analogWrite(motorB_a,  1);
    analogWrite(motorB_b,  1);
  }  
}


uint8_t arduino_logo[] PROGMEM={
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xC0, 0xE0, 0xF0, 0xF8, 0xF8, 0xFC, 0xFC,   // 0x0010 (16) pixels
0xFE, 0xFE, 0x7F, 0x7F, 0x7F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x7F, 0x7F, 0x7F, 0xFE, 0xFE, 0xFE,   // 0x0020 (32) pixels
0xFC, 0xFC, 0xF8, 0xF0, 0xF0, 0xE0, 0xC0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xC0, 0xE0,   // 0x0030 (48) pixels
0xF0, 0xF0, 0xF8, 0xFC, 0xFC, 0xFE, 0xFE, 0xFE, 0x7F, 0x7F, 0x7F, 0x3F, 0x3F, 0x3F, 0x3F, 0x7F,   // 0x0040 (64) pixels
0x7F, 0x7F, 0x7F, 0xFE, 0xFE, 0xFC, 0xFC, 0xF8, 0xF8, 0xF0, 0xE0, 0xC0, 0x86, 0x06, 0x06, 0x00,   // 0x0050 (80) pixels
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xFC, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,   // 0x0060 (96) pixels
0x1F, 0x07, 0x01, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,   // 0x0070 (112) pixels
0x80, 0x80, 0x00, 0x00, 0x01, 0x01, 0x03, 0x0F, 0x1F, 0x3F, 0xFF, 0xFF, 0xFF, 0xFE, 0xFC, 0xFE,   // 0x0080 (128) pixels
0xFF, 0xFF, 0xFF, 0x3F, 0x1F, 0x0F, 0x03, 0x01, 0x01, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0xF8,   // 0x0090 (144) pixels
0xF8, 0xF8, 0xF8, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x01, 0x07, 0x1F, 0xFF, 0xFF, 0xFF,   // 0x00A0 (160) pixels
0xFF, 0xFF, 0xFC, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x7F, 0xFF,   // 0x00B0 (176) pixels
0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x07, 0x07, 0x07, 0x07,   // 0x00C0 (192) pixels
0x07, 0x07, 0x07, 0x07, 0x07, 0x03, 0x00, 0x00, 0x00, 0x00, 0x80, 0xC0, 0xF0, 0xF8, 0xFE, 0xFF,   // 0x00D0 (208) pixels
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xF8, 0xF0, 0xC0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x07,   // 0x00E0 (224) pixels
0x07, 0x07, 0x07, 0x3F, 0x3F, 0x3F, 0x3F, 0x07, 0x07, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0,   // 0x00F0 (240) pixels
0xF0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   // 0x0100 (256) pixels
0x00, 0x00, 0x00, 0x00, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F, 0x7F, 0xFF, 0xFE, 0xFE, 0xFC, 0xFC,   // 0x0110 (272) pixels
0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xFC, 0xFC, 0xFC, 0xFE, 0xFF, 0x7F, 0x7F, 0x3F, 0x3F,   // 0x0120 (288) pixels
0x1F, 0x0F, 0x07, 0x03, 0x01, 0x00, 0x00, 0x00, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x3F, 0x7F,   // 0x0130 (304) pixels
0x7F, 0xFF, 0xFE, 0xFC, 0xFC, 0xFC, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xFC, 0xFC, 0xFE,   // 0x0140 (320) pixels
0xFE, 0xFF, 0x7F, 0x7F, 0x3F, 0x1F, 0x0F, 0x07, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   // 0x0150 (336) pixels
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xC0, 0xC0, 0x80, 0x00, 0x00,   // 0x0160 (352) pixels
0x00, 0x00, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0x81, 0x81, 0x01, 0x01, 0xC1, 0xC1, 0xC0, 0xC0,   // 0x0170 (368) pixels
0xC0, 0xC0, 0x80, 0x00, 0x00, 0x00, 0x80, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0xC0, 0xC0, 0x80, 0x00,   // 0x0180 (384) pixels
0x00, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC1, 0xC1, 0x01, 0x01, 0xC1, 0xC1, 0xC1, 0x01, 0x01,   // 0x0190 (400) pixels
0x01, 0xC1, 0xC1, 0x01, 0x00, 0x00, 0x80, 0xC0, 0xC0, 0xC0, 0xC0, 0x80, 0x80, 0x00, 0x00, 0x00,   // 0x01A0 (416) pixels
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xF8, 0xFF, 0x3F, 0x3B,   // 0x01B0 (432) pixels
0x3F, 0x7F, 0xFE, 0xF0, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x1C, 0x3C, 0xFF, 0xFF, 0xC7, 0x00, 0x00,   // 0x01C0 (448) pixels
0xFF, 0xFF, 0xFF, 0xE0, 0xE0, 0xF1, 0x7F, 0x3F, 0x1F, 0x00, 0x3F, 0x7F, 0xFF, 0xE0, 0xC0, 0xE0,   // 0x01D0 (464) pixels
0xFF, 0x7F, 0x3F, 0x00, 0x00, 0xE0, 0xE0, 0xFF, 0xFF, 0xFF, 0xE0, 0xE0, 0xC0, 0x00, 0x00, 0xFF,   // 0x01E0 (480) pixels
0xFF, 0x07, 0x0F, 0x3E, 0xFC, 0xFF, 0xFF, 0x00, 0x00, 0x3F, 0x7F, 0xFF, 0xE0, 0xC0, 0xE1, 0x7F,   // 0x01F0 (496) pixels
0x7F, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
};
