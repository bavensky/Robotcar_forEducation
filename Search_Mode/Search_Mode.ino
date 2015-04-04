/*******************************************************************************
 * Project  : Robot Car Search mode                                            *
 * Compiler : Arduino 1.0.2                                                    *
 * Board    : Arduino UNO                                                      *
 * Module   : Ultrasonic HC SR04                                               *
 *          : Servo motor                                                      *
 *          : Motor Drive Module (L9110S)                                      *
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
#define motor1A  9  // เชื่อมต่อ pin digital 9 arduino
#define motor1B  6  // เชื่อมต่อ pin digital 6 arduino
#define motor2A  5  // เชื่อมต่อ pin digital 5 arduino
#define motor2B  3  // เชื่อมต่อ pin digital 3 arduino

/***************************  Pin connect Ultrasonics  **************************/
#define echoPin A0  // เชื่อมต่อ pin analog 0 arduino
#define trigPin A1  // เชื่อมต่อ pin analog 1 arduino

/* Create variable */
Servo myservo;            // ประกาศตัวแปรสำหรับเซอร์โว   
long duration, distance;  // ประกาศตัวแปรสำหรับเก็บค่าระยะทาง
int sweep = 0;            // ประกาศตัวแปรสำหรับเก็บค่าการหมุนของเซอร์โว


void setup()
{
  pinMode(echoPin, INPUT);      // ตั้งตัวแปร echoPin ให้เป็น อินพุต
  pinMode(trigPin, OUTPUT);     // ตั้งตัวแปร trigPin ให้เป็น เอาต์พุต
  
  
   
  pinMode(motor1A, OUTPUT);    // ตั้งตัวแปร motor1A ให้เป็น เอาต์พุต
  pinMode(motor1B, OUTPUT);    // ตั้งตัวแปร motor1B ให้เป็น เอาต์พุต
  pinMode(motor2A, OUTPUT);    // ตั้งตัวแปร motor2A ให้เป็น เอาต์พุต
  pinMode(motor2B, OUTPUT);    // ตั้งตัวแปร motor2B ให้เป็น เอาต์พุต
  
  myservo.attach(7);  // ใช้งานเซอร์โวที่ pin 7 arduino
  delay(1000);        // หน่วงเวลาก่อนทำงาน 1 วินาที
}

void loop() 
{  
  for(sweep = 0; sweep <= 180; sweep+=5)  {     // หมุนหน้าจากขวาไปซ้าย
    forword(200);           // หุ่นยนต์เดินหน้า
    myservo.write(sweep);   // กำหนดให้เซอร์โวหมุนตามค่า sweep
    check();                // อัลตร้าโซนิคทำงาน เก็บค่าไว้ในตัวแปร distance
    while(distance < 20)  {  // ถ้าเจอวัตถุในระยะ 20 cm 
      check(); 
      turnleft(200);        // ถอยหลังและหมุนซ้าย
    }   
  }
  
  for(sweep = 180; sweep >= 0; sweep-=5)  {      // หมุนหน้าจากซ้ายไปขวา
    forword(200);          // หุ่นยนต์เดินหน้า
    myservo.write(sweep);  // กำหนดให้เซอร์โวหมุนตามค่า sweep
    check();                
    while(distance < 20)  { // ถ้าเจอวัตถุในระยะ 20 cm 
      check(); 
      turnleft(200);        // ถอยหลังและหมุนซ้าย
    }   
  } 
   
}

/******************************  Check Ultrasonic ******************************/
void check()  // ฟังก์ชั่นย่อยสำหรับเช็คระยะทางโดยใช้อัลตร้าโซนิค
{
  digitalWrite(trigPin, LOW);      // กำหนดให้ขา trigPin เป็นลอจิก 0 
  delayMicroseconds(2);            // หน่วงเวลา 2 ไมโครวินาที
  digitalWrite(trigPin, HIGH);     // กำหนดให้ขา trigPin เป็นลอจิก 1 
  delayMicroseconds(10);           // หน่วงเวลา 10 ไมโครวินาที
  digitalWrite(trigPin, LOW);      // กำหนดให้ขา trigPin เป็นลอจิก 0 
  duration = pulseIn(echoPin, HIGH);  // ใช้ฟังก์ชั่น pulseIn อ่านค่าจาก echoPin และเก็บไว้ในตัวแปร duration 
  distance = (duration/2) / 29.1;    // แปลงหน่วงให้เป็นเซนติเมตร
  delay(40);
}

/******************************  Motor Function ******************************/
void forword(int value)
{
  analogWrite(motor1A,  0);
  analogWrite(motor1B,  value);
  analogWrite(motor2A,  0);
  analogWrite(motor2B,  value);
}
void backword(int value)
{
  analogWrite(motor1A,  value);
  analogWrite(motor1B,  0);
  analogWrite(motor2A,  value);
  analogWrite(motor2B,  0);
}
void turnright(int value)
{
  analogWrite(motor1A,  0);
  analogWrite(motor1B,  value);
  analogWrite(motor2A,  0);
  analogWrite(motor2B,  0);
  
}
void turnleft(int value)
{
  analogWrite(motor1A,  0);
  analogWrite(motor1B,  0);
  analogWrite(motor2A,  value);
  analogWrite(motor2B,  0);
  
}
void stopmotor()
{
  analogWrite(motor1A,  0);
  analogWrite(motor1B,  0);
  analogWrite(motor2A,  0);
  analogWrite(motor2B,  0); 
}
