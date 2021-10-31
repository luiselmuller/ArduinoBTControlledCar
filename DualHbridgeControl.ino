/******************************************************
 * Luisel A. Muller Rodriguez
 * 9/25/2021
 * Program to control 4 motors using 2 h-bridge
 * circuits.
 * 
 * How To Use:
 * Byte = 3   -> Forward
 * Byte = 1/7 -> Turn right 
 * Byte = 2/6 -> Turn left 
 * Byte = 8   -> Reverse
 * Byte = 5/6 -> Turn left 
 * Byte = 4/7 -> Turn right 
 * Byte = 9   -> Stop
 * Byte = d   -> Speed is lowered to a minimum of 160pwm
 * Byte = u   -> Speed is increased to a maximum of 255pwm
 * 
 * === Connect BTModule RX pin to TX pin on arduino ===
 * ===   and BTModule TX pin to RX pin on arduino   ===
 *****************************************************/
//pins used to turn motors on/off
int forwardRight = 5;  //moves the motors on the right side forward
int forwardLeft = 9;  //moves the motors on the left side forward
int backwardRight = 6;  //moves the motors on the right backwards
int backwardLeft = 10; //moves the motors on the left backwards

//min 160pwm - max 255 pwm
int spd = 160;

void setup() 
{
  // put your setup code here, to run once:
  pinMode(forwardRight, OUTPUT);
  pinMode(forwardLeft, OUTPUT);
  pinMode(backwardRight, OUTPUT);
  pinMode(backwardLeft, OUTPUT);
  
  Serial.begin(9600);
}

void loop() 
{
  if(Serial.available() > 0) //if there is data to read
  {
    Serial.println("**BLUETOOTH CONNECTED**");
    char rByte = Serial.read();  //reads the data from the bluetooth module
    
    //Button controls
    switch(rByte)
    {
      //if the input byte is d or u the speed is changed (spd variable)
      case 'd':
        if(spd >= 165)
          spd -= 5;
      break;
      case 'u':
        if(spd <= 250)
          spd += 5;
      break;
      //if the user inputs 3 the backwards pins are turned off and the forward pins are turned on with a
      //small delay in between
      case '3':
        Serial.println("Moving Forward");
        //forward
        analogWrite(backwardRight, 0);
        analogWrite(backwardLeft, 0);
        delay(200);
        analogWrite(forwardRight, spd);
        analogWrite(forwardLeft, spd);
      break;
      //if the user inputs 8 the forward pins are turned off and the backwards pins are turned on with a 
      //small delay in between
      case '8':
        Serial.println("Moving Backward");
        //backward
        analogWrite(forwardRight, 0);
        analogWrite(forwardLeft, 0);
        delay(200);
        analogWrite(backwardRight, spd);
        analogWrite(backwardLeft, spd);
      break;
      //if the user inputs 2 the right side forward pin is turned to spdpwm
      case '2':
        Serial.println("Left");
        //turns left
        analogWrite(backwardRight, 0);
        analogWrite(forwardLeft, 0);
        analogWrite(backwardLeft, 0);
        delay(200);
        analogWrite(forwardRight, spd);
      break;
      //if the user inputs 1 the right side forward pin is turned to spdpwm
      case '1':
        Serial.println("Right");
        //turns right
        analogWrite(forwardRight, 0);
        analogWrite(backwardLeft, 0);
        analogWrite(backwardRight, 0);
        delay(200);
        analogWrite(forwardLeft, spd);
      break;
      //turns left by turning the left motors in reverse and right motors forward
      case '6':
        analogWrite(forwardLeft, 0);
        analogWrite(forwardRight, 0);
        delay(200);
        analogWrite(backwardLeft, spd);
        analogWrite(forwardRight, spd);
      break;
      //turns right by turning the left motors forward and right motors in reverse
      case '7':
        analogWrite(forwardLeft, 0);
        analogWrite(forwardRight, 0);
        delay(200);
        analogWrite(forwardLeft, spd);
        analogWrite(backwardRight, spd);
      break;
      //turns left using reverse on one side
      case '5':
        //left
        analogWrite(forwardRight, 0);
        analogWrite(forwardLeft, 0);
        analogWrite(backwardRight, 0);
        delay(200);
        analogWrite(backwardLeft, spd);
      break;
      //turns right using reverse on one side
      case '4':
        //right
        analogWrite(forwardRight, 0);
        analogWrite(forwardLeft, 0);
        analogWrite(backwardLeft, 0);
        delay(200);
        analogWrite(backwardRight, spd);
      break;
      //if the user inputs 0 the car will stop
      case '9':
        //stops
        analogWrite(forwardRight, 0);
        analogWrite(backwardLeft, 0);
        analogWrite(forwardLeft, 0);
        analogWrite(backwardRight, 0);
        delay(200);
   }
  }
}
