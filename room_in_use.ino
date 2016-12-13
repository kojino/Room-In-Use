#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>
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
int digital_value; // Holds the digital value
int analog_value; // Holds the analog value
char operation; // Holds operation (R, W, ...)
int wait_for_transmission = 5; // Delay in ms in order to receive the serial data
char mode; // Holds the mode (D, A)
int value_to_write; // Holds the value that we want to write
int pin_number; // Holds the pin number
bool emergencyMode = false;

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

void set_pin_mode(int pin_number, char mode){
    /*
     * Performs a pinMode() operation depending on the value of the parameter
     * mode :
     * - I: Sets the mode to INPUT
     * - O: Sets the mode to OUTPUT
     * - P: Sets the mode to INPUT_PULLUP
     */

    switch (mode){
        case 'I':
            pinMode(pin_number, INPUT);
            break;
        case 'O':
            pinMode(pin_number, OUTPUT);
            break;
        case 'P':
            pinMode(pin_number, INPUT_PULLUP);
            break;
    }
}

void digital_read(int pin_number){
    /*
     * Performs a digital read on pin_number and returns the value read to serial
     * in this format: D{pin_number}:{value}\n where value can be 0 or 1
     */

    digital_value = digitalRead(pin_number);
    Serial.print('D');
    Serial.print(pin_number);
    Serial.print(':');
    Serial.println(digital_value); // Adds a trailing \n
}

void analog_read(int pin_number){
    /*
     * Performs an analog read on pin_number and returns the value read to serial
     * in this format: A{pin_number}:{value}\n where value ranges from 0 to 1023
     */

    analog_value = analogRead(pin_number);
    Serial.print('A');
    Serial.print(pin_number);
    Serial.print(':');
    Serial.println(analog_value); // Adds a trailing \n
}

void digital_write(int pin_number, int digital_value){
    /*
     * Performs a digital write on pin_number with the digital_value
     * The value must be 1 or 0
     */
  digitalWrite(pin_number, digital_value);
}

void analog_write(int pin_number, int analog_value){
    /*
   * Performs an analog write on pin_number with the analog_value
   * The value must be range from 0 to 255
     */
  analogWrite(pin_number, analog_value);
}

void loop() 
{
  _delay(1);
  controlLoop();
  if(isPeopleDetected()) {
    movingCount += 1;
    cycleCount += 1;
  } else {
    cycleCount += 1;
  }

//  Serial.print(cycleCount);
  if (cycleCount == 10) {
//    Serial.print(String(movingCount) + " out of " + String(cycleCount) + "detected.");
    if (movingCount >= 5) {
      roomInUse = true;
//      Serial.print("Room In Use");
      turnOnLED(RED);
      turnOffLED(GREEN);
      movingCount = 0;
      cycleCount = 0;
    } else {
      roomInUse = false;
//      Serial.print("Room Is Empty");
      turnOffLED(RED);
      turnOnLED(GREEN);
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


void controlLoop() {
    // Check if characters available in the buffer
    if (Serial.available() > 0) {
        operation = Serial.read();
        if (operation == 'H'){
            if (roomInUse) {
              Serial.print("use");
            } else {
              Serial.print("empty");
            }
        }
        delay(wait_for_transmission); // If not delayed, second character is not correctly read
        mode = Serial.read();
        pin_number = Serial.parseInt(); // Waits for an int to be transmitted
        if (Serial.read()==':'){
            value_to_write = Serial.parseInt(); // Collects the value to be written
        }
        switch (operation){
            case 'R': // Read operation, e.g. RD12, RA4
                if (mode == 'D'){ // Digital read
                    digital_read(pin_number);
                } else if (mode == 'A'){ // Analog read
                    analog_read(pin_number);
        } else {
          break; // Unexpected mode
        }
                break;

            case 'W': // Write operation, e.g. WD3:1, WA8:255
                if (mode == 'D'){ // Digital write
                    digital_write(pin_number, value_to_write);
                } else if (mode == 'A'){ // Analog write
                    analog_write(pin_number, value_to_write);
                } else {
                    break; // Unexpected mode
                }
                break;

            case 'M': // Pin mode, e.g. MI3, MO3, MP3
                set_pin_mode(pin_number, mode); // Mode contains I, O or P (INPUT, OUTPUT or PULLUP_INPUT)
                break;

            case 'E':
                if (mode == '1') {
                    emergencyMode = true;
                } else {
                    emergencyMode = false;
                }                

            default: // Unexpected char
                break;
        }
    }
}

