char incomingByte = 0;   
int motorleft1=3;
int motorleft2=5;
int motorright1=6;
int motorright2=9;

void setup() 
{
  Serial.begin(19200);
  pinMode(motorleft1, OUTPUT);   
  pinMode(motorleft2, OUTPUT);   
  pinMode(motorright1, OUTPUT);   
  pinMode(motorright2, OUTPUT);  
}

void loop() 
{
  if (Serial.available() > 0) 
  {
    incomingByte = Serial.read();
    Serial.println(incomingByte);
    
    if(incomingByte == 'A')
      {
        left(200);
      }   
      else if(incomingByte == 'D')
      {    
        right(200);
      }
      else if(incomingByte == 'W')
      {
        forword(200);
      }     
      else if(incomingByte == 'S')
      {
        backword(200);
      }
      else if(incomingByte == 'X')
      {
        stopmotor();
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
