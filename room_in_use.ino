
/*****************************************************************************/
//  Function: If the sensor detects the moving people in it's detecting range,
//        the Grove - LED is turned on.Otherwise, the LED is turned off.
//  Hardware: Grove - PIR Motion Sensor, Grove - LED
//  Arduino IDE: Arduino-1.0
/*******************************************************************************/
/*macro definitions of PIR motion sensor pin and LED pin*/
#define PIR_MOTION_SENSOR 2//Use pin 2 to receive the signal from the module 
#define GREEN 4//the Grove - LED is connected to D4 of Arduino
#define RED 8//the Grove - LED is connected to D4 of Arduino
int movingCount = 0;
int cycleCount = 0;
bool roomInUse = false;

void setup()
{
  Serial.begin(9600); // Serial Port at 9600 baud
  Serial.setTimeout(100); 
  pinsInit();
}

void _delay(float seconds){
    long endTime = millis() + seconds * 1000;
    while(millis() < endTime) {
        continue;
    }
}

void loop() 
{
  _delay(1);
  if(isPeopleDetected()) {
    movingCount += 1;
    cycleCount += 1;
  } else {
    cycleCount += 1;
  }

  Serial.print(cycleCount);
  if (cycleCount == 10) {
    Serial.print(String(movingCount) + " out of " + String(cycleCount) + "detected.");
    if (movingCount >= 5) {
      roomInUse = true;
      Serial.print("Room In Use");
      turnOnLED(GREEN);
      turnOffLED(RED);
      movingCount = 0;
      cycleCount = 0;
    } else {
      roomInUse = false;
      Serial.print("Room Is Empty");
      turnOffLED(GREEN);
      turnOnLED(RED);
      movingCount = 0;
      cycleCount = 0;
    }
    
  }
 
  
}
void pinsInit()
{
  pinMode(PIR_MOTION_SENSOR, INPUT);
  pinMode(RED,OUTPUT);
  pinMode(GREEN,OUTPUT);
}
void turnOnLED(int LED)
{
  digitalWrite(LED,HIGH);
}
void turnOffLED(int LED)
{
  digitalWrite(LED,LOW);
}
/***************************************************************/
/*Function: Detect whether anyone moves in it's detecting range*/
/*Return:-boolean, ture is someone detected.*/
boolean isPeopleDetected()
{
  int sensorValue = digitalRead(PIR_MOTION_SENSOR);
  if(sensorValue == HIGH)//if the sensor value is HIGH?
  {
    return true;//yes,return ture
  }
  else
  {
    return false;//no,return false
  }
}
