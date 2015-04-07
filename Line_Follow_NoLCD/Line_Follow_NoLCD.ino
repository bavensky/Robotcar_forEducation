/*******************************************************************************
 * Project  : Robot Car Line Follow No LCD Display                             *
 * Compiler : Arduino 1.5.5-r2                                                 *
 * Board    : Arduino UNO                                                      *
 * Module   : Line Tracking Sensor                                             *
 *          : Motor Drive Module (L9110S)                                      *
 * Author   : Bavensky :3                                                      *
 * E-Mail   : Aphirak_Sang-ngenchai@hotmail.com                                *
 * Date     : 06/04/2015 [dd/mm/yyyy]                                          *
 *          : ลายเส้นสีดำ ขนาด 3 เซนติเมตร
 *******************************************************************************/

// เรียกใช้งานไลบรารี่สำหรับใช้งานเซอร์โว 
#include <Servo.h> 

/*****************************  Motor Diagram  *********************************
              ________
            /          \
Motor1      |          |      Motor2
      ||||| |          | |||||
      ||||| |          | ||||| 
      ||||| |          | |||||
            |          |
            |__________|
            
/***************************  Pin connect Motor  ********************************/
#define motor1A  9  // เชื่อมต่อขา digital 9 (PWM) ของบอร์ด arduino
#define motor1B  6  // เชื่อมต่อขา digital 6 (PWM) ของบอร์ด arduino
#define motor2A  5  // เชื่อมต่อขา digital 5 (PWM) ของบอร์ด arduino
#define motor2B  3  // เชื่อมต่อขา digital 3 (PWM) ของบอร์ด arduino

/*********************  Pin connect Line Tracking Sensor  **********************/
#define SENSOR_1  A0  
#define SENSOR_2  A1
#define SENSOR_3  A2  
#define SENSOR_4  A3

//  ประกาศตัวแปรสำหรับเก็บค่าจากปุ่มกดบน Shield
#define SWITCH    A5

//  ประกาศตัวแปรสำหรับเก็บค่าจากเซ็นเซอร์
int sensorValue1 = 0;    
int sensorValue2 = 0;  
int sensorValue3 = 0;    
int sensorValue4 = 0;  

//  ประกาศตัวแปรสำหรับเก็บค่าจากปุ่มกด
int switch_all = 0;
char key;

//  สร้างตัวแปรสำหรับใช้งานเซอร์โว
Servo gripper; 

void setup() { 
  
  gripper.attach(7);           // กำหนดเซอร์โวให้ใช้งานที่ขา 7 
  
  pinMode(SENSOR_1, INPUT);    // กำหนดใหตัวแปร SENSOR_1 ให้เป็น อินพุต
  pinMode(SENSOR_2, INPUT);    // กำหนดใหตัวแปร SENSOR_2 ให้เป็น อินพุต
  pinMode(SENSOR_3, INPUT);    // กำหนดใหตัวแปร SENSOR_3 ให้เป็น อินพุต
  pinMode(SENSOR_4, INPUT);    // กำหนดใหตัวแปร SENSOR_4 ให้เป็น อินพุต
  
  pinMode(motor1A, OUTPUT);    // กำหนดใหตัวแปร motor1A ให้เป็น เอาต์พุต
  pinMode(motor1B, OUTPUT);    // กำหนดใหตัวแปร motor1B ให้เป็น เอาต์พุต
  pinMode(motor2A, OUTPUT);    // กำหนดใหตัวแปร motor2A ให้เป็น เอาต์พุต
  pinMode(motor2B, OUTPUT);    // กำหนดใหตัวแปร motor2B ให้เป็น เอาต์พุต
}

void loop() {
/************  Drive Motor  ****************************
Motor Right
  motor1A  =  1  Go Reverse
  motor1B  =  1  Go Forward
Motor Left  
  motor2A  =  1  Go Reverse
  motor2B  =  1  Go Forward
********************************************************/

  // อ่านค่าจากเซนเซอร์
  read_sensor();
  
  // เมื่อเซ็นเซอร์ 2 และ 3 เจอเส้น หุ่นยนต์จะเดินหน้า
  if(sensorValue1 == 0 && sensorValue2 == 1 && sensorValue3 == 1 && sensorValue4 == 0) forword(128);
  // เมื่อเซ็นเซอร์ 4 เจอเส้น หุ่นยนต์จะเลี้ยวขวามาก
  if(sensorValue1 == 0 && sensorValue2 == 0 && sensorValue3 == 0 && sensorValue4 == 1) turnright(128);  
  // เมื่อเซ็นเซอร์ 3 เจอเส้น หุ่นยนต์จะเลี้ยวขวาเล็กน้อย 
  if(sensorValue1 == 0 && sensorValue2 == 0 && sensorValue3 == 1 && sensorValue4 == 0) turnright(75);
  // เมื่อเซ็นเซอร์ 1 เจอเส้น หุ่นยนต์จะเลี้ยวซ้ายมาก
  if(sensorValue1 == 1 && sensorValue2 == 0 && sensorValue3 == 0 && sensorValue4 == 0) turnleft(128);
  // เมื่อเซ็นเซอร์ 2 เจอเส้น หุ่นยนต์จะเลี้ยวซ้ายเล็กน้อย
  if(sensorValue1 == 0 && sensorValue2 == 1 && sensorValue3 == 0 && sensorValue4 == 0) turnleft(75);
  // เมื่อเซนเซอร์ทั้ง 4 ตัวไม่เจอเส้นหรือเจอเส้นพร้อมกันทั้งหมด หุ่นยนต์จะหยุดอยู่กับที่
  if(sensorValue1 == 0 && sensorValue2 == 0 && sensorValue3 == 0 && sensorValue4 == 0) stopmotor();
  if(sensorValue1 == 1 && sensorValue2 == 1 && sensorValue3 == 1 && sensorValue4 == 1) stopmotor(); 
}

/***************************  Sensor Function ***************************/
void read_sensor()  {
  sensorValue1 = digitalRead(SENSOR_1);  // อ่านค่าดิจิตอลจากตัวแปร SENSOR_1 และเก็บค่าไว้ในตัวแปร sensorValue1
  sensorValue2 = digitalRead(SENSOR_2);  // อ่านค่าดิจิตอลจากตัวแปร SENSOR_2 และเก็บค่าไว้ในตัวแปร sensorValue2
  sensorValue3 = digitalRead(SENSOR_3);  // อ่านค่าดิจิตอลจากตัวแปร SENSOR_3 และเก็บค่าไว้ในตัวแปร sensorValue3
  sensorValue4 = digitalRead(SENSOR_4);  // อ่านค่าดิจิตอลจากตัวแปร SENSOR_4 และเก็บค่าไว้ในตัวแปร sensorValue4
}

/***************************  Switch Function ***************************/
void read_switch()  {
  switch_all  =  analogRead(SWITCH);  // อ่านค่าแอนะล็อกจากตัวแปร SWITCH และเก็บค่าไว้ในตัวแปร switch_all  
  
  if(switch_all > 850)  {    // ถ้าค่า switch_all มากกว่า 850 ให้ตัวแปร key เก็บค่าอักขระ X
    delay(200);
    key = 'X';
  }
  if(switch_all > 720 && switch_all < 800)  {  // ถ้าค่า switch_all มากกว่า 720 และน้อยกว่า  800 ให้ตัวแปร key เก็บค่าอักขระ M
    delay(200);
    key = 'M';
  }
  if(switch_all > 450  &&  switch_all < 550)  {  // ถ้าค่า switch_all มากกว่า 450 และน้อยกว่า  550 ให้ตัวแปร key เก็บค่าอักขระ S
    delay(200);
    key = 'S';
  }
  if(switch_all > 300  &&  switch_all < 400)  {  // ถ้าค่า switch_all มากกว่า 300 และน้อยกว่า  400 ให้ตัวแปร key เก็บค่าอักขระ U
    delay(200);
    key = 'U';
  }  
  if(switch_all < 200)  {     // ถ้าค่า switch_all น้อยกว่า 200 ให้ตัวแปร key เก็บค่าอักขระ D
    delay(200);  
    key = 'D';
  }
}

/******************************  Motor Function ******************************/
void forword(int value)  {
  analogWrite(motor1A,  0);
  analogWrite(motor1B,  value);
  analogWrite(motor2A,  0);
  analogWrite(motor2B,  value);
}
void turnright(int value)  {
  analogWrite(motor1A,  0);
  analogWrite(motor1B,  0);
  analogWrite(motor2A,  0);
  analogWrite(motor2B,  value);  
}
void turnleft(int value)  {
  analogWrite(motor1A,  0);
  analogWrite(motor1B,  value);
  analogWrite(motor2A,  0);
  analogWrite(motor2B,  0);
}
void stopmotor()  {
  analogWrite(motor1A,  0);
  analogWrite(motor1B,  0);
  analogWrite(motor2A,  0);
  analogWrite(motor2B,  0); 
}

