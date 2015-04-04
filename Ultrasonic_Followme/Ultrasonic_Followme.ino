/*******************************************************************************
 * Project  : Robot Car mode follow me                                         *
 * Compiler : Arduino 1.0.2                                                    *
 * Board    : Arduino UNO                                                      *
 * Module   : Ultrasonic HC SR04                                               *
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
/***************************  Pin connect Motor  ********************************/
#define motor1A  9  // เชื่อมต่อ pin digital 9 arduino
#define motor1B  6  // เชื่อมต่อ pin digital 6 arduino
#define motor2A  5  // เชื่อมต่อ pin digital 5 arduino
#define motor2B  3  // เชื่อมต่อ pin digital 3 arduino

/***************************  Pin connect Ultrasonics  **************************/
#define echoPin1  A0  // เชื่อมต่อ echoPin1 to pin analog 0 arduino
#define trigPin1  A1  // เชื่อมต่อ trigPin1 to pin analog 1 arduino

#define echoPin2  A2  // เชื่อมต่อ echoPin2 to pin analog 2 arduino
#define trigPin2  A3  // เชื่อมต่อ trigPin2 to pin analog 3 arduino

#define echoPin3  A4  // เชื่อมต่อ echoPin3 to pin analog 4 arduino
#define trigPin3  A5  // เชื่อมต่อ trigPin3 to pin analog 5 arduino

/* Create variable */  
// ประกาศตัวแปรสำหรับเก็บค่าระยะทางทั้ง 3 เซนซอร์
long duration1, distance1;
long duration2, distance2;
long duration3, distance3;

void setup() 
{
  
  pinMode(echoPin1, INPUT);     // กำหนดให้ตัวแปร echoPin1 ให้เป็น อินพุต
  pinMode(echoPin2, INPUT);     // กำหนดใหตัวแปร echoPin2 ให้เป็น อินพุต
  pinMode(echoPin3, INPUT);     // กำหนดใหตัวแปร echoPin3 ให้เป็น อินพุต
  pinMode(trigPin1, OUTPUT);    // กำหนดใหตัวแปร trigPin1 ให้เป็น เอาต์พุต
  pinMode(trigPin2, OUTPUT);    // กำหนดใหตัวแปร trigPin2 ให้เป็น เอาต์พุต
  pinMode(trigPin3, OUTPUT);    // กำหนดใหตัวแปร trigPin3 ให้เป็น เอาต์พุต
  
  pinMode(motor1A, OUTPUT);    // กำหนดใหตัวแปร motor1A ให้เป็น เอาต์พุต
  pinMode(motor1B, OUTPUT);    // กำหนดใหตัวแปร motor1B ให้เป็น เอาต์พุต
  pinMode(motor2A, OUTPUT);    // กำหนดใหตัวแปร motor2A ให้เป็น เอาต์พุต
  pinMode(motor2B, OUTPUT);    // กำหนดใหตัวแปร motor2B ให้เป็น เอาต์พุต
}

void loop() 
{  
/********************  Flowing Robot **********************************************/
  check1();    // ฟังก์ชั่นย่อยสำหรับเช็คระยะทางโดยใช้อัลตร้าโซนิคตัวที่ 1
  check2();    // ฟังก์ชั่นย่อยสำหรับเช็คระยะทางโดยใช้อัลตร้าโซนิคตัวที่ 2
  check3();    // ฟังก์ชั่นย่อยสำหรับเช็คระยะทางโดยใช้อัลตร้าโซนิคตัวที่ 3

  
  // ถ้าเซนเซอร์ตัวที่ 1 (ด้านซ้ายสุด) เจอวัตถุ จะหมุนซ้าย
  if(distance1 <= 30 && distance2 > 30 && distance3 >30 ) turnleft(200);
  // ถ้าเซนเซอร์ตัวที่ 2 (ตรงกลาง) เจอวัตถ จะเดินหน้า
  if(distance2 <= 30 && distance1 > 30 && distance3 > 30)  forword(200);
  // ถ้าเซนเซอร์ตัวที่ 2 (ด้านขวาสุด) เจอวัตถ จะหมุนขวา
  if(distance3 <= 30 && distance1 > 30 && distance2 > 30)  turnright(200);
  // ถ้าเซนเซอร์ทั้ง 3 ตัวไม่เจอวัตถุ
  if(distance1 > 30 && distance2 > 30 && distance3 > 30)  stopmotor();
/**********************************************************************************/
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
  analogWrite(motor1A,  value);
  analogWrite(motor1B,  0);
  analogWrite(motor2A,  0);
  analogWrite(motor2B,  value);
  
}
void turnleft(int value)
{
  analogWrite(motor1A,  0);
  analogWrite(motor1B,  value);
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

/******************************  Check Ultrasonic ******************************/
void check1()
{
  digitalWrite(trigPin1, LOW);  
  delayMicroseconds(2); 
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);  
  duration1 = pulseIn(echoPin1, HIGH);
  distance1 = (duration1/2) / 29.1;
}

void check2()
{
  digitalWrite(trigPin2, LOW);  
  delayMicroseconds(2); 
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);  
  duration2 = pulseIn(echoPin2, HIGH);
  distance2 = (duration2/2) / 29.1;
}

void check3()
{
  digitalWrite(trigPin3, LOW);  
  delayMicroseconds(2); 
  digitalWrite(trigPin3, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin3, LOW);  
  duration3 = pulseIn(echoPin3, HIGH);
  distance3 = (duration3/2) / 29.1;
}


