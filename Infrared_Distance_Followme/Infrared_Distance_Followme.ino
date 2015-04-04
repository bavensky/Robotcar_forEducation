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
#define infrared1  A0  // เชื่อมต่อ infrared1 to pin analog 0 arduino
#define infrared2  A2  // เชื่อมต่อ infrared2 to pin analog 2 arduino
#define infrared3  A4  // เชื่อมต่อ infrared3 to pin analog 4 arduino


/* Create variable */  
// ประกาศตัวแปรสำหรับเก็บค่าระยะทางทั้ง 3 เซนซอร์
int sensor1 = 0, sensor2 = 0, sensor3 = 0;

void setup() 
{
  Serial.begin(9600);
  
  pinMode(infrared1, INPUT);    // กำหนดใหตัวแปร infrared1 ให้เป็น อินพุต
  pinMode(infrared2, INPUT);    // กำหนดใหตัวแปร infrared2 ให้เป็น อินพุต
  pinMode(infrared3, INPUT);    // กำหนดใหตัวแปร infrared3 ให้เป็น อินพุต
  
  pinMode(motor1A, OUTPUT);    // กำหนดใหตัวแปร motor1A ให้เป็น เอาต์พุต
  pinMode(motor1B, OUTPUT);    // กำหนดใหตัวแปร motor1B ให้เป็น เอาต์พุต
  pinMode(motor2A, OUTPUT);    // กำหนดใหตัวแปร motor2A ให้เป็น เอาต์พุต
  pinMode(motor2B, OUTPUT);    // กำหนดใหตัวแปร motor2B ให้เป็น เอาต์พุต
}

void loop() 
{
  sensor1 = digitalRead(infrared1);
  sensor2 = digitalRead(infrared2);
  sensor3 = digitalRead(infrared3);
  Serial.print(sensor1); Serial.print("\t");
  Serial.print(sensor2); Serial.print("\t");
  Serial.println(sensor3);
  
/********************  Flowing Robot **********************************************/
  // ถ้าเซนเซอร์ตัวที่ 1 (ด้านซ้ายสุด) เจอวัตถุ จะหมุนขวา
  if(sensor1 == 0 && sensor2 == 1 && sensor3 == 1 ) turnright(255);
  // ถ้าเซนเซอร์ทั้ง 3 ตัวไม่เจอวัตถุ หุ่นยนต์จะเดินหน้า
  if(sensor2 == 1 && sensor1 == 1 && sensor3 == 1)  forword(255);
  // ถ้าเซนเซอร์ตัวที่ 3 (ด้านขวาสุด) เจอวัตถ จะหมุนซ้าย
  if(sensor3 == 0 && sensor1 == 1 && sensor2 == 1  )  turnleft(255);
  // ถ้าเซนเซอร์ตัวที่ 2 (ตรงกลาง) เจอวัตถุ จะถอยหลัง
  if(sensor1 == 1 && sensor2 == 0 && sensor3 == 1)  backword(255);
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

