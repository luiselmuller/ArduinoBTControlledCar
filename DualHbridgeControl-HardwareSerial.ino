/***********************************************************************************************************
 * Luisel A. Muller Rodriguez
 * 9/25/2021
 * Program to control 4 motors using 2 h-bridge
 * circuits by sending commands from an app through bluetooth.
 *
 * How To Use:
 * Byte = 3   -> Forward
 * Byte = 1/7 -> Turn right 
 * Byte = 2/6 -> Turn left 
 * Byte = 8   -> Reverse
 * Byte = 5/6 -> Turn left 
 * Byte = 4/7 -> Turn right 
 * Byte = 9   -> Stop
 * Byte = d   -> Speed is lowered 5pwm at a time, minimum of 160pwm
 * Byte = u   -> Speed is increased 5pwm at a time, maximum of 255pwm
 * 
 * Some bytes like 1 and 7 have the same action, they both move the car right but do 
 * it in different ways. When the data received is '1' then it will turn right only
 * using the motors on one side. When the data received is '7' then it will turn 
 * right using one side to move forward and the other side to move backwards.
 * More details about this are in the rest of the comments.
 *
 * === Connect BTModule RX pin to TX pin on arduino ===
 * ===   and BTModule TX pin to RX pin on arduino   ===
 * Bluetooth module used: HC-05
 *
 * The app used to control the arduino is available at:
 * https://gallery.appinventor.mit.edu/?galleryid=ad552982-a765-438e-b923-764d1356e364
 *
 * The APK is at: https://github.com/luiselmuller/ArduinoBTControlledCar/blob/main/ArduinoProjectELEN.apk
 **********************************************************************************************************/

/*****************************************************************************
* Arduino library that lets you read the dht sensor values without blocking 
* the execution of other code.
* ---------------------------------------------------------------------------
* From: https://github.com/olewolf/DHT_nonblocking
*****************************************************************************/
#include <dht_nonblocking.h>
#define DHT_SENSOR_TYPE DHT_TYPE_11   //This line defines the type of the sensor as a dht11.

static const int DHT_SENSOR_PIN = 2;  //Sets the sensor pin to pin 2 on the arduino, this pin will read its data.
DHT_nonblocking dht_sensor( DHT_SENSOR_PIN, DHT_SENSOR_TYPE );


/* Pins used to turn motors on/off. */
int forwardRight = 5;  //Moves the motors on the right side forward.
int forwardLeft = 9;  //Moves the motors on the left side forward.
int backwardRight = 6;  //Moves the motors on the right backwards.
int backwardLeft = 10; //Moves the motors on the left backwards.

/******************************************************************
* This variable stores the pwm used on the arduino pins, basically
* changing the pwm also changes the speed of the motors, so this
* variable basically controls the speed of the motors.
* -- Minimum PWM: 160, Maximum PWM: 255 --
******************************************************************/
int spd = 160;

void setup() 
{
  //Setting the pints used to control the motors as output pins
  pinMode(forwardRight, OUTPUT);
  pinMode(forwardLeft, OUTPUT);
  pinMode(backwardRight, OUTPUT);
  pinMode(backwardLeft, OUTPUT);
  
  //Baud rate set at 9600, the Bluetooth module also has a Baud rate of 9600.
  Serial.begin(9600);
}

/********************************************************************
 * Poll for a measurement, keeping the state machine alive.  Returns
 * true if a measurement is available.
 *******************************************************************/
static bool measure_environment( float *temperature, float *humidity )
{
  static unsigned long measurement_timestamp = millis( );

  /* Measure once every four seconds. */
  if( millis( ) - measurement_timestamp > 3000ul )
  {
    if( dht_sensor.measure( temperature, humidity ) == true )
    {
      measurement_timestamp = millis( );
      return( true );
    }
  }
  return( false );
}

void loop() 
{
  //If there is data to read.
  if(Serial.available()) 
  {
    //The data that is read is compared.
    switch(Serial.read())
    {
      /**************************************************************************************
      * If the data read is 'd' then the variable spd is reduced by 5 only if spd is greater
      * or equal to 165, this is to avoid the pwm value going under the minimum of 160. 
      * This reduction of the spd variable reduces the speed of the motors.
      **************************************************************************************/
      case 'd':
        if(spd >= 165)
          spd -= 5;
      break;
        
      /**************************************************************************************
      * If the data read is 'u' then the variable spd is increased by 5 only if spd is less 
      * or equal to 250, this limits the value to 255 to avoid errors since that is the
      * maximum pwm for the arduino.
      **************************************************************************************/
      case 'u':
        if(spd <= 250)
          spd += 5;
      break;

      /*****************************************************************************************************
      * If the data received is '3', the pins that move the car backwards are turned off and then the pins
      * that move it forward are turned on. This moves the car forward.
      *****************************************************************************************************/
      case '3':
        analogWrite(backwardRight, 0);
        analogWrite(backwardLeft, 0);
        analogWrite(forwardRight, spd);
        analogWrite(forwardLeft, spd);
      break;
      
      /*******************************************************************************************************
      * If the data received is '8' then the pins that move the car forward are turned off and the pins that
      * move it backwards are turned on. This moves the car backwards.
      *******************************************************************************************************/
      case '8':
        analogWrite(forwardRight, 0);
        analogWrite(forwardLeft, 0);
        analogWrite(backwardRight, spd);
        analogWrite(backwardLeft, spd);
      break;
      
      /****************************************************************************************************
      * If the data received is '2' the pins that move the left motors forward & backwards and 
      * the right motors backwards are turned off, the remaining pin is on. This makes the car turn to 
      * the left.
      ****************************************************************************************************/
      case '2':
        analogWrite(backwardRight, 0);
        analogWrite(forwardLeft, 0);
        analogWrite(backwardLeft, 0);
        analogWrite(forwardRight, spd);
      break;
      
      /****************************************************************************************************
      * If the data received is '1' the pins that move the right motors forward & backwardds and 
      * the left motors backwards are turned off, the remaining pin is on. This makes the car turn to 
      * the right.
      ****************************************************************************************************/
      case '1':
        analogWrite(forwardRight, 0);
        analogWrite(backwardLeft, 0);
        analogWrite(backwardRight, 0);
        analogWrite(forwardLeft, spd);
      break;
      
      /**************************************************************************************************
      * If the data received is '6' then the pins that move the right motors backwards and left motors
      * forward are turned off while the others are on. This causes the car to rotate to the left.
      **************************************************************************************************/
      case '6':
        analogWrite(forwardLeft, 0);
        analogWrite(backwardRight, 0);
        analogWrite(backwardLeft, spd);
        analogWrite(forwardRight, spd);
      break;
      
      /**************************************************************************************************
      * If the data received is '7' then the pins that move the left motors backwards and right motors
      * forward are turned off while the others are on. This causes the car to rotate to the right.
      **************************************************************************************************/
      case '7':
        analogWrite(backwardLeft, 0);
        analogWrite(forwardRight, 0);
        analogWrite(forwardLeft, spd);
        analogWrite(backwardRight, spd);
      break;
      
      /***************************************************************************************************
      * If the data received is '5' all of the pins except the one moving the left motors backwards
      * are turned off. This makes the car turn left using reverse.
      ***************************************************************************************************/
      case '5':
        analogWrite(forwardRight, 0);
        analogWrite(forwardLeft, 0);
        analogWrite(backwardRight, 0);
        analogWrite(backwardLeft, spd);
      break;
      
      /***************************************************************************************************
      * If the data received is '4' all of the pins except the one moving the right motors backwards
      * are turned off. This makes the car turn right using reverse.
      ***************************************************************************************************/
      case '4':
        analogWrite(forwardRight, 0);
        analogWrite(forwardLeft, 0);
        analogWrite(backwardLeft, 0);
        analogWrite(backwardRight, spd);
      break;
      
      /*****************************************************************************************
      * When the data received is 0 all of the pins will be turned off and will stop the car.
      *****************************************************************************************/
      case '9':
        //stops
        analogWrite(forwardRight, 0);
        analogWrite(backwardLeft, 0);
        analogWrite(forwardLeft, 0);
        analogWrite(backwardRight, 0);
      break;
   }
  }
  
  //Variables used to store temperature and humidity measurements.
  float temperature;  
  float humidity;

  /***********************************************************************************************
  * Using the previous function that was declared to check if there is a measurement available,
  * the data from the sensor is taken and stored in the variables created before. Then, the 
  * temperature data is converted from Celsius to Fahrenheit. To send the data properly through
  * the bluetooth module and then to the app the data is concatenated into one string separated 
  * by the character "|". --Ex. ( 68F|56% )
  * ----------------------------------------------------------------------------------------------
  * The app takes a string like 68F|56% and splits it at the "|" character, then it assigns
  * the temperature and humidity values to a list which then are set as the text for some 
  * labels.
  ***********************************************************************************************/
  if( measure_environment( &temperature, &humidity ) == true )
  {
    //Temperature conversion from Celsius to Fahrenheit.
    float temp = (temperature * 9/5) + 32;
    
    //String that will hold the string to be sent to the app.
    String out = "";
    //Concatenation of both sensor readings.
    out.concat(temp);
    out.concat("|");
    out.concat(humidity);
    //Sending the data to the app through the hardware serial of the arduino and HC-05 module.
    Serial.println(out);  
   }
}
