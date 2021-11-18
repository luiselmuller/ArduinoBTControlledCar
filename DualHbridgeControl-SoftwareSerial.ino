/**********************************************************************
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
 * Byte = d   -> Speed is lowered 5pwm at a time, minimum of 160pwm
 * Byte = u   -> Speed is increased 5pwm at a time, maximum of 255pwm
 * 
 * === Connect BTModule RX pin to TX pin on arduino ===
 * ===   and BTModule TX pin to RX pin on arduino   ===
 *********************************************************************/
#include <SoftwareSerial.h>
SoftwareSerial Blue(11,12); //RX || TX

#include <dht_nonblocking.h>
#define DHT_SENSOR_TYPE DHT_TYPE_11

static const int DHT_SENSOR_PIN = 2;
DHT_nonblocking dht_sensor( DHT_SENSOR_PIN, DHT_SENSOR_TYPE );


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
  Blue.begin(9600);
}

/*
 * Poll for a measurement, keeping the state machine alive.  Returns
 * true if a measurement is available.
 */
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
  if(Blue.available()) //if there is data to read
  {
    //Button controls
    switch(Blue.read())
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
        //forward
        analogWrite(backwardRight, 0);
        analogWrite(backwardLeft, 0);
        analogWrite(forwardRight, spd);
        analogWrite(forwardLeft, spd);
      break;
      
      //if the user inputs 8 the forward pins are turned off and the backwards pins are turned on with a 
      //small delay in between
      case '8':
        //backward
        analogWrite(forwardRight, 0);
        analogWrite(forwardLeft, 0);
        analogWrite(backwardRight, spd);
        analogWrite(backwardLeft, spd);
      break;
      
      //if the user inputs 2 the right side forward pin is turned to spdpwm
      case '2':
        //turns left
        analogWrite(backwardRight, 0);
        analogWrite(forwardLeft, 0);
        analogWrite(backwardLeft, 0);
        analogWrite(forwardRight, spd);
      break;
      
      //if the user inputs 1 the right side forward pin is turned to spdpwm
      case '1':
        //turns right
        analogWrite(forwardRight, 0);
        analogWrite(backwardLeft, 0);
        analogWrite(backwardRight, 0);
        analogWrite(forwardLeft, spd);
      break;
      
      //turns left by turning the left motors in reverse and right motors forward
      case '6':
        analogWrite(forwardLeft, 0);
        analogWrite(forwardRight, 0);
        analogWrite(backwardLeft, spd);
        analogWrite(forwardRight, spd);
      break;
      
      //turns right by turning the left motors forward and right motors in reverse
      case '7':
        analogWrite(forwardLeft, 0);
        analogWrite(forwardRight, 0);
        analogWrite(forwardLeft, spd);
        analogWrite(backwardRight, spd);
      break;
      
      //turns left using reverse on one side
      case '5':
        //left
        analogWrite(forwardRight, 0);
        analogWrite(forwardLeft, 0);
        analogWrite(backwardRight, 0);
        analogWrite(backwardLeft, spd);
      break;
      
      //turns right using reverse on one side
      case '4':
        //right
        analogWrite(forwardRight, 0);
        analogWrite(forwardLeft, 0);
        analogWrite(backwardLeft, 0);
        analogWrite(backwardRight, spd);
      break;
      
      //if the user inputs 0 the car will stop
      case '9':
        //stops
        analogWrite(forwardRight, 0);
        analogWrite(backwardLeft, 0);
        analogWrite(forwardLeft, 0);
        analogWrite(backwardRight, 0);
      break;
   }
  }
  float temperature;
  float humidity;

  //temp/hum measurement
  if( measure_environment( &temperature, &humidity ) == true )
  {
    //temperature conversion from Celsius to Fahrenheit
    float temp = (temperature * 9/5) + 32;
    
    //sending data to app
    String out = "";
    out.concat(temp);
    out.concat("|");
    out.concat(humidity);
    Blue.println(out);
   }
}
