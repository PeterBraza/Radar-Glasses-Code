/*#include <SoftwareSerial.h>
#include "TFMini.h"
// Setup software serial port 
SoftwareSerial mySerial(3, 2);      // Uno RX (TFMINI TX), Uno TX (TFMINI RX)
TFMini Tfmini;

void setup () {
  // Step 1: Initialize hardware serial port (serial debug port)
  Serial.begin(115200);
  // wait for serial port to connect. Needed for native USB port only
  while(!Serial);
     
  Serial.println("Initializing...");

  // Step 2: Initialize the data rate for the Software Serial port
  mySerial.begin(TFMINI_BAUDRATE);

  // Step 3: Initialize the TF Mini sensor
  Tfmini.begin(&mySerial);    
}

void loop () {
  // Take one TF Mini distance measurement
  int dist = Tfmini.getDistance();
  int strength = Tfmini.getRecentSignalStrength();

  // Display the measurement
  Serial.print(dist);
  Serial.print(" cm      sigstr : ");
  Serial.println(strength);

  // Wait some short time before taking the next measurement
  delay(250);  
}*/
#include <SoftwareSerial.h>
#include "TFMini.h"
TFMini tfmini;
 
SoftwareSerial SerialTFMini(2, 3);          //The only value that matters here is the first one, 2, Rx
 
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
  Serial.begin(115200);       //Initialize hardware serial port (serial debug port)
  while (!Serial);            // wait for serial port to connect. Needed for native USB port only
  Serial.println ("Initializing...");
  SerialTFMini.begin(TFMINI_BAUDRATE);    //Initialize the data rate for the SoftwareSerial port
  tfmini.begin(&SerialTFMini);            //Initialize the TF Mini sensor
}
 
void loop()
{
  int distance = 0;
  int strength = 0;
 
  getTFminiData(&distance, &strength);
  while (!distance)
  {
    getTFminiData(&distance, &strength);
    if (distance)
    {
      Serial.print(distance);
      Serial.print("cm\t");
      Serial.print("strength: ");
      Serial.println(strength);
    }
  }
  delay(100);
}
