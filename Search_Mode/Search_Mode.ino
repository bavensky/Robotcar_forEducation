/*******************************************************************************
 * Project  : Robot Car Search mode                                            *
 * Compiler : Arduino 1.0.2                                                    *
 * Board    : Arduino UNO                                                      *
 * Module   : Ultrasonic HC SR04                                               *
 *          : LiquidCrystal                                                    *
 * Author   : Bavensky :3                                                      *
 * E-Mail   : Aphirak_Sang-ngenchai@hotmail.com                                *
 * Date     : 25/03/2014 [dd/mm/yyyy]                                          *
 *******************************************************************************/
 
/*****************************  Motor Diagram  *********************************
              ________
            /          \
Motor1      |          |      Motor2
      ||||| |          | |||||
      ||||| |          | ||||| 
      ||||| |          | |||||
            |          |
            |__________|
********************************************************************************/

#include <Servo.h>

/***************************  Pin connect Motor  ********************************/
#define motor1A  5
#define motor1B  4
#define motor2A  3
#define motor2B  2

/***************************  Pin connect Ultrasonics  **************************/
#define echoPin A0
#define trigPin A1

/* Create variable */
Servo myservo;
long duration, distance;
int sweep = 0;


void setup()
{
  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);
  
  pinMode(motor1A, OUTPUT);
  pinMode(motor1B, OUTPUT);
  pinMode(motor2A, OUTPUT);
  pinMode(motor2B, OUTPUT);
  
  myservo.attach(7);
}

void loop() 
{
  for(sweep = 0; sweep < 90; sweep+=5)  {
    myservo.write(sweep);
    check();
    if(distance < 10)  {
      forword(200);
    }
    delay(200);
  }

}

/******************************  Check Ultrasonic ******************************/
void check()
{
  digitalWrite(trigPin, LOW);  
  delayMicroseconds(2); 
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);  
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;
}

/******************************  Motor Function ******************************/
void forword(int value)
{
  analogWrite(motor1A,  value);
  analogWrite(motor1B,  0);
  analogWrite(motor2A,  0);
  analogWrite(motor2B,  value);
}
void backword()
{
  analogWrite(motor1A,  0);
  analogWrite(motor1B,  255);
  analogWrite(motor2A,  255);
  analogWrite(motor2B,  0);
}
void turnleft(int value)
{
  analogWrite(motor1A,  value);
  analogWrite(motor1B,  0);
  analogWrite(motor2A,  0);
  analogWrite(motor2B,  0);
  
}
void turnright(int value)
{
  analogWrite(motor1A,  0);
  analogWrite(motor1B,  0);
  analogWrite(motor2A,  0);
  analogWrite(motor2B,  value);
  
}
void stopmotor()
{
  analogWrite(motor1A,  0);
  analogWrite(motor1B,  0);
  analogWrite(motor2A,  0);
  analogWrite(motor2B,  0); 
}
