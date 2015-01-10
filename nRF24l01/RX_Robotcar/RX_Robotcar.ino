/* nRF24L01 
 http://arduino-info.wikispaces.com/Nrf24L01-2.4GHz-HowTo
   1 - GND
   2 - VCC 3.3V !!! NOT 5V
   3 - CE to Arduino pin 9
   4 - CSN to Arduino pin 10
   5 - SCK to Arduino pin 13
   6 - MOSI to Arduino pin 11
   7 - MISO to Arduino pin 12
   8 - UNUSED 
   11 - Motorleft1 to pin 2
   12 - Motorleft2 to pin 3
   13 - Motorright1 to pin 4
   14 - Motorright2 to pin 5
   
*/
   
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define CE_PIN   9
#define CSN_PIN 10

const uint64_t pipe = 0xE8E8F0F0E1LL; 
RF24 radio(CE_PIN, CSN_PIN); 
int joystick[2];  

int motorleft1=2,  motorleft2=3,  motorright1=4,  motorright2=5;
int speed_left, speed_right, speed_forword, speed_backword;

void setup()   
{
  Serial.begin(9600);

  radio.begin();
  radio.openReadingPipe(1,pipe);
  radio.startListening();
  
  pinMode(motorleft1, OUTPUT);   
  pinMode(motorleft2, OUTPUT);   
  pinMode(motorright1, OUTPUT);   
  pinMode(motorright2, OUTPUT); 
}

void loop()   
{
  rx();
}

void rx()
{
  if ( radio.available() )
  {
    bool done = false;
    while (!done)
    {
      done = radio.read( joystick, sizeof(joystick) );
      speed_left     = map(joystick[0], 513, 0, 0, 255);
      speed_right    = map(joystick[0], 513, 1023, 0, 255);
      speed_forword  = map(joystick[1], 506, 1023, 0, 255);
      speed_backword = map(joystick[1], 506, 0, 0, 255);
      
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
  analogWrite(motorleft1,  value);
  analogWrite(motorleft2,  LOW);

  analogWrite(motorright1,  LOW);
  analogWrite(motorright2,  value);
}

void backword(int value)
{
  analogWrite(motorleft1,  LOW);
  analogWrite(motorleft2,  value);
   
  analogWrite(motorright1,  value);
  analogWrite(motorright2,  LOW);
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

