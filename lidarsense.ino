#include <SoftwareSerial.h>
#include "TFMini.h"

int motorPin = 5; //motor transistor is connected to pin 5
TFMini tfmini;
SoftwareSerial SerialTFMini(10, 11); //green goes to 10, white goes to 11 (for wiring).

void getTFminiData(int* distance, int* strength)
{
  static char i = 0;
  char j = 0;
  int checksum = 0;
  static int rx[9];
  if (SerialTFMini.available())
  {
    rx[i] = SerialTFMini.read();
    if (rx[0] != 0x59)
    {
      i = 0;
    }
    else if (i == 1 && rx[1] != 0x59)
    {
      i = 0;
    }
    else if (i == 8)
    {
      for (j = 0; j < 8; j++)
      {
        checksum += rx[j];
      }
      if (rx[8] == (checksum % 256))
      {
        *distance = rx[2] + rx[3] * 256;
        *strength = rx[4] + rx[5] * 256;
      }
      i = 0;
    }
    else
    {
      i++;
    }
  }
}
void setup()
{
  Serial.begin(115200);
  pinMode(motorPin, OUTPUT);
  while(!Serial);
  SerialTFMini.begin(TFMINI_BAUDRATE);
  tfmini.begin(&SerialTFMini);  
  //pinMode(AO, INPUT);
}
//

//max input frequency is 10
//input is hertz (1/seconds)
unsigned long long int i = 1;
void loop()
{
  //gets the value of the "dial" or potentiometer for sensitivity
  //Serial.println(cast);
  //Serial.println(c);
  //delay(1000);
 /* digitalWrite(5, HIGH); //vibrate
  delay(1000);  // delay one second
  digitalWrite(5, LOW);  //stop vibrating
  delay(1000); //wait 50 seconds.*/
  //vibrate(100);
  int distance = 0;
  int strength = 0;

  getTFminiData(&distance, &strength);
  while (!distance)
  {
    getTFminiData(&distance, &strength);
    if (distance && distance>-1)
    {
      int potVal = analogRead(A0);
      Serial.print(" ");
      Serial.print(distance);
      Serial.print(" ");
      Serial.println(potVal);
      
      int operand = ((distance)/(potVal*20)) + 1;

      (i%operand == 0 && potVal>100 && distance < 600) ? digitalWrite(motorPin, HIGH),  delay(24), digitalWrite(motorPin, LOW), i++: i++;
      delay(24);
      (i == 4294967294) ? i = 1: i;
      //double v = 0;
    }
  }
 // delay(200);
}
