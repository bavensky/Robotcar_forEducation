/*******************************************************************************
 * Project  : Robot Car For Education                                          *
 * Part     : nRF24l01_RX                                                      *
 * Compiler : Arduino 1.5.5-r2                                                 *
 * Board    : Arduino UNO                                                      *
 * Author   : Bavensky :3                                                      *
 * E-Mail   : Apiruk_Sang-ngenchai@hotmail.com                                 *
 * Date     : 10/01/2558 [dd/mm/yyyy]                                          *
 * Read more: http://arduino-info.wikispaces.com/Nrf24L01-2.4GHz-HowTo         *
 *******************************************************************************/

/******  How to connect nRF24L01 to arduino board ********/
//   1 - GND
//   2 - VCC 3.3V !!! NOT 5V
//   3 - CE to Arduino pin 9
//   4 - CSN to Arduino pin 10
//   5 - SCK to Arduino pin 13
//   6 - MOSI to Arduino pin 11
//   7 - MISO to Arduino pin 12
//   8 - UNUSED 
//   11 - Motorleft1 to pin 2
//   12 - Motorleft2 to pin 3
//   13 - Motorright1 to pin 4
//   14 - Motorright2 to pin 5
/***********************************************************/
   
#include <SPI.h>            // เรียกใช้งานไลบรารี่การเชื่อมต่อแบบ SPI
#include <nRF24L01.h>       // เรียกใช้งานไลบรารี่โมดูล nRF24L01
#include <RF24.h>

#define CE_PIN   4          // กำหนดขา CE ต่อเข้าขา 9 ของบอร์ดอาดุยโน่
#define CSN_PIN 10          // กำหนดขา CSN ต่อเข้าขา 10 ของบอร์ดอาดุยโน่

const uint64_t pipe = 0xE8E8F0F0E1LL; 
RF24 radio(CE_PIN, CSN_PIN);     

int joystick[2];            // ประกาศตัวแปรสำหรับเรียกใช้งานจอยสติ๊ก

// motorleft คือมอเตอร์ทางด้านซ้าย และ motorright คือมอเตอร์ทางด้านขวา
int motorleft1 = 2;  // กำหนดให้ตัวแปร motorleft1 เชื่อมต่อขา 2 ของบอร์ดอาดุยโน่
int motorleft2 = 3;  // กำหนดให้ตัวแปร motorleft2 เชื่อมต่อขา 3 ของบอร์ดอาดุยโน่
int motorright1 = 5;  // กำหนดให้ตัวแปร motorright1 เชื่อมต่อขา 4 ของบอร์ดอาดุยโน่
int motorright2 = 6;  // กำหนดให้ตัวแปร motorright2 เชื่อมต่อขา 5 ของบอร์ดอาดุยโน่
int speed_left, speed_right, speed_forword, speed_backword;       // ประกาศตัวแปรแบบ interger สำหรับเก็บค่าที่อ่านจากจอยสติ๊ก

void setup()   
{
  Serial.begin(19200);
  radio.begin();            // เริ่มใช้งาน nRF24l01               
  radio.openReadingPipe(1,pipe);
  radio.startListening();
  
  pinMode(motorleft1, OUTPUT);      // กำหนดให้ตัวแปร motorleft1 เป็นเอาต์พุต     
  pinMode(motorleft2, OUTPUT);      // กำหนดให้ตัวแปร motorleft2 เป็นเอาต์พุต   
  pinMode(motorright1, OUTPUT);     // กำหนดให้ตัวแปร motorright1 เป็นเอาต์พุต   
  pinMode(motorright2, OUTPUT);     // กำหนดให้ตัวแปร motorright2 เป็นเอาต์พุต   
}

void loop()   
{
  rx();      // เรียกใช้ฟังก์ชั่นย่อย
}

void rx()    // ประกาศฟังก์ชั่นย่อยชื่อ rx
{
  if ( radio.available() )  // เริ่มการรับค่า
  {
    bool done = false;
    while (!done)
    {
      done = radio.read( joystick, sizeof(joystick) );        // อ่านค่าแล้วนำมาเก็บไว้ในตัวแปรแบบอาเรย์ชื่อ joystick
      speed_left     = map(joystick[0], 513, 0, 0, 255);
      speed_right    = map(joystick[0], 513, 1023, 0, 255);
      speed_forword  = map(joystick[1], 506, 1023, 0, 255);
      speed_backword = map(joystick[1], 506, 0, 0, 255);
      
      Serial.print(speed_forword);
      Serial.print("    ");
      Serial.println(speed_backword);
      if(speed_left > 0)
      {
        left(speed_left);
      }   
      else if(speed_right > 0)
      {    
        right(speed_right);
      }
      else if(speed_forword > 0)
      {
        forword(speed_forword);
      }     
      else if(speed_backword > 0)
      {
        backword(speed_backword);
      }
      else
      {
        stopmotor();
      }
    } 
  }
}
void forword(int value)
{
//  analogWrite(motorleft1,  LOW);
//  analogWrite(motorleft2,  value);

  analogWrite(motorright1,  value);
  analogWrite(motorright2,  LOW);
}

void backword(int value)
{
//  analogWrite(motorleft1,  LOW);
//  analogWrite(motorleft2,  value);
   
  analogWrite(motorright1,  LOW);
  analogWrite(motorright2,  value);
}

void left(int value)
{
  analogWrite(motorright1,  LOW);
  analogWrite(motorright2,  value);
}

void right(int value)
{
  analogWrite(motorleft1,  value);
  analogWrite(motorleft2,  LOW);
}

void stopmotor()
{
  analogWrite(motorleft1,  LOW);
  analogWrite(motorleft2,  LOW);
 
  analogWrite(motorright1,  LOW);
  analogWrite(motorright2,  LOW);
}

