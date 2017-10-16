#include <SoftwareSerial.h>
#include "CaptorSet.h"
#include "Motors.h"

// XBee's DOUT (TX) is connected to pin 11 (Arduino's Software RX)
// XBee's DIN (RX) is connected to pin 12 (Arduino's Software TX)
SoftwareSerial XBee(11, 12); // RX, TX

char RIGHT_MOTOR_UP[4] = {'R', 'M', 'U', 'P'};
char LEFT_MOTOR_UP[4] = {'L', 'M', 'U', 'P'};
char RIGHT_MOTOR_DOWN[4] = {'R', 'M', 'D', 'W'};
char LEFT_MOTOR_DOWN[4] = {'L', 'M', 'D', 'W'};
char CAPTORS[5] = {'C', 'P', 'T', 'R', '\0'};

void setup()
{
  // Set up both ports at 115200 baud. This value is most important
  // for the XBee. Make sure the baud rate matches the config
  // setting of your XBee.
  XBee.begin(115200);
  Serial.begin(115200);
  initCaptors();
  initMotors();
}


void loop() {
  if (XBee.available() > 3) {
    //Assuming orders are always given with 4 characters
    char receivedQuerry[5] = {' ', ' ', ' ', ' ', '\0'};
    for (int i = 0; i < 4; i++) {
      receivedQuerry[i] = XBee.read();
    }
    Serial.println(receivedQuerry);
    Serial.println(strcmp(CAPTORS, receivedQuerry));
    //
    if (strcmp(CAPTORS, receivedQuerry) == 0) {
      int * Distances;
      Distances = getDistance_1();
      char buf[29];
      sprintf(buf, "%04i-%04i-%04i-%04i-%04i\0", Distances[0], Distances[1], Distances[2], Distances[3], Distances[4]);
      Serial.println(buf);
      XBee.write(buf);
    }
  }
  int * Distances;
  Distances = getDistances();
  char buf[29];
  Serial.println(Distances[0]);
  sprintf(buf, "%03i-%03i-%03i-%03i-%03i\0", Distances[0], Distances[1], Distances[2], Distances[3], Distances[4]);
  Serial.println(buf);
}
