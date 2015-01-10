int up_button = 2;
int down_button = 4;
int left_button = 5;
int right_button = 3;
int start_button = 6;
int select_button = 7;
int analog_button = 8;
int x_axis = A0;
int y_axis = A1;

int buttons[] = {up_button, down_button, left_button, right_button, start_button, select_button, analog_button};

byte oldButtonState = 0;
byte up = 0;
byte a=0, b=0, c=0, d=0, e=0, f=0, k=0;
byte data[1];

   
void setup() 
{
  Serial.begin(19200);
  for (int i; i < 7; i++)
  {
   pinMode(buttons[i], INPUT);
   digitalWrite(buttons[i], HIGH);
  }
}

void loop() 
{
// Up
   a = digitalRead(up_button);
   if(a == 0)
   {
     delay(100);
     data[0] = 'W';
     Serial.write(data,1);
   }
// Right   
   b = digitalRead(right_button);
   if(b == 0)
   {
     delay(100);
     data[0] = 'D';
     Serial.write(data,1);
   }
// Down   
   c = digitalRead(down_button);
   if(c == 0)
   {
     delay(100);
     data[0] = 'S';
     Serial.write(data,1);
   }
// Left   
   d = digitalRead(left_button);
   if(d == 0)
   {
     delay(100);
     data[0] = 'A';
     Serial.write(data,1);
   }
   
   f = digitalRead(select_button);
   if(f == 0)
   {
     delay(100);
     data[0] = 'X';
     Serial.write(data,1);
   }
   
   if( a == 1 &&  b == 1 && c == 1 && d ==1)
   {
     data[0] = 'X';
     Serial.write(data,1);
   }

 
  /*Serial.print("A:"),Serial.print(digitalRead(up_button));Serial.print(",");
  Serial.print("C:"),Serial.print(digitalRead(down_button));Serial.print(",");
  Serial.print("D:"),Serial.print(digitalRead(left_button));Serial.print(",");
  Serial.print("B:"),Serial.print(digitalRead(right_button));Serial.print(",");
  Serial.print("E:"),Serial.print(digitalRead(start_button));Serial.print(",");
  Serial.print("F:"),Serial.print(digitalRead(select_button));Serial.print(",");
  Serial.print("K:"),Serial.print(digitalRead(analog_button));Serial.print(",");
  Serial.print("X:"),Serial.print(analogRead(x_axis));Serial.print(",");
  Serial.print("Y:"),Serial.println(analogRead(y_axis));*/
}
