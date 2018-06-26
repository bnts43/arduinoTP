#include "avdweb_VirtualDelay.h"

bool connectionIsSuccessful = false;

// 1- component settings for microphone KY-037 or KY-038
int Analog_Entry = A0; // relative noise value
int Digital_Entry = 3; // returns if maximum value has been outreached on A0

float Analog;
int Digital;

// 2- component settings for red and green led KY-011
int Led_Red = 10;
int Led_Green = 11;

// 3- settings for connecting a network 
String NETWORK_KEY = "777D7EE5DB70B3880A062ED14F5568E6";
String MY_INFO = "B7B7"; // used for NI and MY info on network

// 4- personal information (only useful for documentation)
String MY_SH = "0013A200";
String MY_SL = "417C2574";

// 5- settings for connecting a peer
String Distant_MY="24A4";
String DISTANT_SH = "0013A200";
String DISTANT_SL = "417C2574";



void setup ()
{

    //settings pin mode / behavior
    pinMode (Analog_Entry, INPUT);
    pinMode (Digital_Entry, INPUT);
    pinMode (Led_Red, OUTPUT); 
    pinMode (Led_Green, OUTPUT); 
    
    Serial.begin (9600); // Serial output with 9600 bps
    // turn on led first (red)
    switchLeds();

    // loading newtork configuration settings with Auto
    configuration("AutoAndConnect");

    // launching Xbee configuration and connection
    connectionIsSuccessful = connectDistantXBee();

}

void loop ()
{
  // test if connection was successful
    if (connectionIsSuccessful) {
      // Values will be read from digital and analog entries
      Analog = analogRead (Analog_Entry)  * (5.0 / 1023.0);
      // for any information on the way the definitive value is determined, see:
      // http://sensorkit.en.joy-it.net/index.php?title=KY-037_Microphone_sensor_module_(high_sensitivity)
      Digital = digitalRead (Digital_Entry);
        
      delay(2000);
      Serial.print("Neuts > ");Serial.print(Analog,4);Serial.println(" V");
      switchLeds();
           
    } else {
      // if connection failed:
      Serial.println("Connection failed!");
      // will wait undefinitely
      while(!Serial.available()) {}
    }
    
}





