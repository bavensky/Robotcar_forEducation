/*******************************************************************************
 * Project  : Robot Car mode follow me                                         *
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
#define motor1A  9  // เชื่อมต่อ pin digital 9 arduino
#define motor1B  6  // เชื่อมต่อ pin digital 6 arduino
#define motor2A  5  // เชื่อมต่อ pin digital 5 arduino
#define motor2B  3  // เชื่อมต่อ pin digital 3 arduino

/***************************  Pin connect IRsensor  ***************************/
#define  IR1  A0  // เชื่อมต่อ IR1 to pin analog 0 arduino
#define  IR2  A1  // เชื่อมต่อ IR2 to pin analog 1 arduino
#define  IR3  A2  // เชื่อมต่อ IR3 to pin analog 2 arduino
#define  IR4  A3  // เชื่อมต่อ IR4 to pin analog 3 arduino

// ประกาศตัวแปรสำหรับเก็บค่าของเซนเซอร์ทั้ง 4 ตัว
int line1=0, line2=0, line3=0, line4=0;

void setup() 
{
  pinMode(IR1, INPUT);     // กำหนดให้ตัวแปร IR1 ให้เป็น อินพุต
  pinMode(IR2, INPUT);     // กำหนดให้ตัวแปร IR2 ให้เป็น อินพุต
  pinMode(IR3, INPUT);     // กำหนดให้ตัวแปร IR3 ให้เป็น อินพุต 
  pinMode(IR4, INPUT);     // กำหนดให้ตัวแปร IR4 ให้เป็น อินพุต
  
  pinMode(motor1A, OUTPUT);    // ตั้งตัวแปร motor1A ให้เป็น เอาต์พุต
  pinMode(motor1B, OUTPUT);    // ตั้งตัวแปร motor1B ให้เป็น เอาต์พุต
  pinMode(motor2A, OUTPUT);    // ตั้งตัวแปร motor2A ให้เป็น เอาต์พุต
  pinMode(motor2B, OUTPUT);    // ตั้งตัวแปร motor2B ให้เป็น เอาต์พุต
}

void loop() 
{ 
/******************  Autonomouse Tracking line ********************************/
  line1 = digitalRead(IR1);       // อ่านค่าจากเซนเซอร์ IR1 เก็บไว้ในตัวแปร line1
  line2 = digitalRead(IR2);       // อ่านค่าจากเซนเซอร์ IR2 เก็บไว้ในตัวแปร line2
  line3 = digitalRead(IR3);       // อ่านค่าจากเซนเซอร์ IR3 เก็บไว้ในตัวแปร line3
  line4 = digitalRead(IR4);       // อ่านค่าจากเซนเซอร์ IR4 เก็บไว้ในตัวแปร line4
  
  // ถ้าเซนเซอร์ line1 และ line4 ไม่เจอ แต่ เซนเซอร์ line2 และ line3 เจอ หุ่นยนต์จะเดินหน้า
  if(line1 == 0 && line2 == 1 && line3 == 1 && line4 == 0 )  forword(150);
  
  // หุ่นยนต์เลี้ยวไปทางซ้าย
  if(line1 == 1 && line2 == 1 && line3 == 0 && line4 == 0 )  turnleft(128);
  if(line1 == 1 && line2 == 1 && line3 == 1 && line4 == 0 )  turnleft(150);
  
  // หุ่นยนต์เลี้ยวไปทางขวา
  if(line1 == 0 && line2 == 0 && line3 == 1 && line4 == 1 )  turnright(128);
  if(line1 == 0 && line2 == 1 && line3 == 1 && line4 == 1 )  turnright(150);
  
  // หุ่นยนตหยุดการเคลื่อนไหว
  if(line1 == 1 && line2 == 1 && line3 == 1 && line4 == 1 )  stopmotor();
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

