// This code shows a method for reading the distance measured by 
// ToughSonic ultrasonic sensors provided by Senix.
// The ultrasonic sensor tested is ToughSonic 3 (RS232 model) and should be connected 
// through a MAX232-based conversion circuit to a 5V microcontroller serial port.
// The MAX232 chip is connected to a hardware serial port in 
// the Teensy 3.5 mircrocontroller development system.
// Output: ultrasonic distance measured in millimeters.

// Notes
// ToughSonic sensor is configured via SenixVIEW software to work 
// in continous mode with 100 msec interval.

// Connections: 
// Yellow wire  ................. pin 14 (T1OUT) in MAX232 IC  
// Gray wire    ................. pin 13 (R1IN) in MAX232 IC
// Brown wire   ................. +24 VDC - Power supply
// Blue wire    ................. GND - Common ground

// pin 11 (T1IN)  in MAX232 IC ... pin 32 (TX4) in Teensy 3.5 
// pin 12 (R1OUT) in MAX232 IC ... pin 31 (RX4) in Teensy 3.5 

// serial port 4
#define ultra_serial Serial4

//////////////////////////////////////////  SETUP
void setup()
{
  // initialize PC serial port (9600 BR)
  Serial.begin(9600);
  delay(100);
  
  // initialize hardware serial port for ultrasonic sensor (9600 BR) 
  ultra_serial.begin(9600);
  delay(100);
}//end setup

///////////////////////////////////////  MAIN LOOP
void loop()
{
  // use this loop to read a single ultrasonic distance 
  // from a continously streaming sensor
  int count_measure = 0;
  while (true)
  {
    count_measure = readUltra();
    if (count_measure != -1) break;
  }//end while  
      
  // convert the measured count to distance in inches
  float inches_measure = count_measure * 0.003384;
  
  // convert distance from inches to cm 
  float centis_measure = inches_measure * 2.54;
  
  // convert distance from cm to mm 
  float millis_measure = centis_measure * 10;

  // report measurement in millimeters 
  Serial.print("Distance in mm: ");
  Serial.println(millis_measure);   
} //end loop

///////////////////////////////////// read ultrasonic distance (low-level)
int readUltra()
{
  int message_size = 0;
  String ultra_message = "";
  
  // loop hardware serial port to get the message complete
  while (true)
  {
    // check if data is available on the hardaware serial port
    if (ultra_serial.available() > 0)
    {
      // read character
      char u = ultra_serial.read();
      // check for end of a message
      if (u == '\r')
      {
        break;
      } else
      { // concat char to message String
        // and increment the message size
        ultra_message.concat(u);
        message_size = message_size + 1;
      }//end else
    }//end if
  }//end while
  // check if a complete message was received
  if (message_size == 5)
  {
    return ultra_message.toInt();
  } else
  {
    return -1;
  }//end if-else
}//end function
