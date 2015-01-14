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
#define motor1A  5
#define motor1B  4
#define motor2A  3
#define motor2B  2

/***************************  Pin connect Ultrasonics  **************************/
#define echoPin1 A0
#define trigPin1 A1
#define echoPin2 A2
#define trigPin2 A3
#define echoPin3 A4
#define trigPin3 A5

/* Create variable */
long duration1, distance1;
long duration2, distance2;
long duration3, distance3;

/***************************  Pin connect IRsensor  ***************************/
#define IR1 6
#define IR2 7
#define IR3 8
#define IR4 9

int line1=0, line2=0, line3=0, line4=0;

void setup() 
{
  Serial.begin (19200);
  
  pinMode(echoPin1, INPUT);
  pinMode(echoPin2, INPUT);
  pinMode(echoPin3, INPUT);
  pinMode(trigPin1, OUTPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(trigPin3, OUTPUT);
  
  pinMode(IR1, INPUT);
  pinMode(IR2, INPUT);
  pinMode(IR3, INPUT); 
  pinMode(IR4, INPUT);
  
  pinMode(motor1A, OUTPUT);
  pinMode(motor1B, OUTPUT);
  pinMode(motor2A, OUTPUT);
  pinMode(motor2B, OUTPUT);
}

void loop() 
{ 
  
  

  
  
/******************  Autonomouse Tracking line ********************************/
  line1 = digitalRead(IR1);
  line2 = digitalRead(IR2);
  line3 = digitalRead(IR3);
  line4 = digitalRead(IR4);
  Serial.print(line1);
  Serial.print(line2);
  Serial.print(line3);
  Serial.println(line4);
  
  if(line1 == 0 && line2 == 1 && line3 == 1 && line4 == 0 )  forword(150);
  
  if(line1 == 1 && line2 == 1 && line3 == 0 && line4 == 0 )  turnleft(128);
  if(line1 == 1 && line2 == 1 && line3 == 1 && line4 == 0 )  turnleft(150);
  
  if(line1 == 0 && line2 == 0 && line3 == 1 && line4 == 1 )  turnright(128);
  if(line1 == 0 && line2 == 1 && line3 == 1 && line4 == 1 )  turnright(150);
  
  if(line1 == 1 && line2 == 1 && line3 == 1 && line4 == 1 )  stopmotor();

  
  
/**********************************************************************************/
  
/********************  Flowing Robot **********************************************
  check1();
  check2();
  check3();

  if(distance1 <= 30 && distance2 > 30 && distance3 > 30 ) turnleft();
  if(distance2 <= 30 && distance1 > 30 && distance3 > 30)  forword();
  if(distance3 <= 30 && distance1 > 30 && distance2 > 30)  turnright();
  if(distance1 > 30 && distance2 > 30 && distance3 > 30)  stopmotor();
**********************************************************************************/
}


/************************************ Motor Control ******************************/
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


