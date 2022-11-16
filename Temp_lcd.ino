/***********************************************
step-1-connect 3v-vcc(lcd),grnd-grnd,scl-d1,sda-d2,
step-2-temp-vin + 3v -resistor-470 -data line of temp sensor +D4(gpio)

*********************/




// First we include the libraries
//#include <ESP32WiFi.h> for esp8266
#include "WiFi.h" //for esp32
#include <OneWire.h> 
#include <DallasTemperature.h>
#include <LiquidCrystal_I2C.h>
#include <ThingSpeak.h>
#include <WiFiManager.h>


WiFiClient client;//wifi setup

long myChannelNumber = 1915341;   //channel name
const char myWriteAPIKey[] = "YYHEPNA4YVXEH7TZ"; //api key
/********************************************************************/
// Data wire is plugged into pin 2 on the Arduino 
#define ONE_WIRE_BUS 2 //pin D3(gpio-0)//D4 (gpio-2) attchin esp8266 for esp32-d2 at 2 
/********************************************************************/
// Setup a oneWire instance to communicate with any OneWire devices  
// (not just Maxim/Dallas temperature ICs) 
OneWire oneWire(ONE_WIRE_BUS); 
/********************************************************************/
// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);
/********************************************************************/ 
LiquidCrystal_I2C lcd(0x3F,20,4);  // set the LCD address to 0x3F for a 20 chars and 4 line display

void setup(void) 
{ 
   WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP
  // it is a good practice to make sure your code sets wifi mode how you want it.

  // put your setup code here, to run once:
  Serial.begin(115200);

  //WiFiManager, Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wm;

  // reset settings - wipe stored credentials for testing
  // these are stored by the esp library
  wm.resetSettings();

  // Automatically connect using saved credentials,
  // if connection fails, it starts an access point with the specified name ( "AutoConnectAP"),
  // if empty will auto generate SSID, if password is blank it will be anonymous AP (wm.autoConnect())
  // then goes into a blocking loop awaiting configuration and will return success result

  bool res;
  // res = wm.autoConnect(); // auto generated AP name from chipid
  res = wm.autoConnect("Hydroponics"); // anonymous ap
 // res = wm.autoConnect("Hydrophonics", "password"); // password protected ap
  // put your setup code here, to run once:

if (!res) {
    Serial.println("Failed to connect");
    // ESP.restart();
  }
  else {
    //if you get here you have connected to the WiFi
    Serial.println("connected...yeey :)");

    Serial.println("Dallas Temperature IC Control Library Demo"); 
 // Start up the library 
 sensors.begin(); 

 //lcd code

 lcd.init();
  lcd.clear();         
  lcd.backlight();      // Make sure backlight is on

    //To begin sensor and thingspeak
    sensors.begin();
    ThingSpeak.begin(client);
  } 
 
} 
void loop(void) 
{ 
 // call sensors.requestTemperatures() to issue a global temperature 
 // request to all devices on the bus 
/********************************************************************/
 Serial.print(" Requesting temperatures..."); 
 sensors.requestTemperatures(); // Send the command to get temperature readings 
 sensors.requestTemperatures();


  float t = sensors.getTempCByIndex(0);
  Serial.println("Temp: " + (String) t);
 
/********************************************************************/

  lcd.setCursor(2,0);   //Set cursor to character 2 on line 0
  lcd.print("Temp is:" + (String) t );

   lcd.setCursor(2,1);   //Move cursor to character 2 on line 1
  //  ds lcd.print (sensors.getTempCByIndex(0));

//thingspeak
   ThingSpeak.writeField(myChannelNumber, 1, t, myWriteAPIKey);
   delay(1000); 
} 