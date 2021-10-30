/******************************************************
 * Luisel A. Muller Rodriguez
 * 9/25/2021
 * Program to control 4 motors using 2 h-bridge
 * circuits.
 * 
 * How To Use:
 * Byte = 3 -> Forward
 * Byte = 1 -> Turn right with wheels turning forward
 * Byte = 2 -> Turn left with wheels turning forward
 * Byte = 8 -> Reverse
 * Byte = 5 -> Turn left with wheels turning backward
 * Byte = 4 -> Turn right with wheels turning forward
 * Byte = 9 -> Stop
 * 
 *****************************************************/
 
//#include<SoftwareSerial.h>
//SoftwareSerial Blue(2, 3); //RX | TX

//pins used to turn motors on/off
int forwardRight = 5;  //moves the motors on the right side forward
int forwardLeft = 9;  //moves the motors on the left side forward
int backwardRight = 6;  //moves the motors on the right backwards
int backwardLeft = 10; //moves the motors on the left backwards

//min 160pwm - max 255 pwm
int spd = 255;

void setup() 
{
  // put your setup code here, to run once:

  pinMode(forwardRight, OUTPUT);
  pinMode(forwardLeft, OUTPUT);
  pinMode(backwardRight, OUTPUT);
  pinMode(backwardLeft, OUTPUT);
  
  Serial.begin(9600);
  //serial communication with bluetooth module
  //Blue.begin(9600);
}

void loop() 
{
  // put your main code here, to run repeatedly
  //read data from arduino to send to bluetooth module 
  //if(Serial.available())
  //{
    //char data = Serial.readString();
    //Blue.println(data);
  //}
  
  if(Serial.available() > 0) //if there is data to read
  {
    Serial.println("**BLUETOOTH CONNECTED**");
    char rByte = Serial.read();  //reads the data from the bluetooth module
    
    //Button controls
    if(rByte == 'b')
    {
        //if the user inputs 1 the backwards pins are turned off and the forward pins are turned on with a
      //small delay in between
      if(rByte == '3')
      {
        Serial.println("Moving Forward");
        //forward
        analogWrite(backwardRight, 0);
        analogWrite(backwardLeft, 0);
        delay(500);
        analogWrite(forwardRight, spd);
        analogWrite(forwardLeft, spd);
        
      }
      //if the user inputs 2 the forward pins are turned off and the backwards pins are turned on with a 
      //small delay in between
      if(rByte == '8')
      {
        Serial.println("Moving Backward");
      //backward
        analogWrite(forwardRight, 0);
        analogWrite(forwardLeft, 0);
        delay(500);
        analogWrite(backwardRight, spd);
        analogWrite(backwardLeft, spd);
        
      }
      //if the user inputs 3 the right side forward pin is turned to spdpwm
      if(rByte == '2')
      {
        Serial.println("Left");
        //turns left
        analogWrite(backwardRight, 0);
        analogWrite(forwardLeft, 0);
        analogWrite(backwardLeft, 0);
        delay(500);
        analogWrite(forwardRight, spd);
        
      }
      //if the user inputs 4 the right side forward pin is turned to spdpwm
      if(rByte == '1')
      {
        Serial.println("Right");
        //turns right
        analogWrite(forwardRight, 0);
        analogWrite(backwardLeft, 0);
        analogWrite(backwardRight, 0);
        delay(500);
        analogWrite(forwardLeft, spd);
  
      }
      //turns left using reverse on one side
      if(rByte == '5')
      {
        Serial.println("Moving Backward Left");
      //left
        analogWrite(forwardRight, 0);
        analogWrite(forwardLeft, 0);
        analogWrite(backwardRight, 0);
        delay(500);
        analogWrite(backwardLeft, spd);
      }
      //turns right using reverse on one side
      if(rByte == '4')
      {
        Serial.println("Moving Backward Right");
      //right
        analogWrite(forwardRight, 0);
        analogWrite(forwardLeft, 0);
        analogWrite(backwardLeft, 0);
        delay(500);
        analogWrite(backwardRight, spd);
      }
      //if the user inputs 0 the car will stop
      if(rByte == '9')
      {
        //stops
        analogWrite(forwardRight, 0);
        analogWrite(backwardLeft, 0);
        analogWrite(forwardLeft, 0);
        analogWrite(backwardRight, 0);
        delay(500);
  
      }
    }
    //joystick
    if(rByte == 'a')
    {
      
    }
  }


}
