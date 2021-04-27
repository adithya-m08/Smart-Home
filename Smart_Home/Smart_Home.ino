/***************************************************
  Google Assistant and Adafuit IO Based Home Automation.

  This Project uses a Nodemcu to connect to Adafruit 
  MQTT Server over a Wifi-Network with internet access, 
  and a 5V relay module to interface with the appliances.
  This Project also uses IFTTT service to translate the 
  commands given by the user to the Google Assistant, to
  pre-defined keywords and publish them into a feed. 

  Written by Adithya.M.
  All text above must be included in any redistribution
 ****************************************************/

#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"         //Libraries
#include "Adafruit_MQTT_Client.h"

#define WIFI_SSID "<wifiname>"      //wifi name
#define WIFI_PASS "<wifipassword>"  //wifi password

#define MQTT_SERV "io.adafruit.com"       //server address
#define MQTT_PORT 1883                    //server port
#define MQTT_NAME "<adafruit username>"   //adafruit username
#define MQTT_PASS "<adafruit passkey>"    //adafruit passkey

WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, MQTT_SERV, MQTT_PORT, MQTT_NAME, MQTT_PASS);
Adafruit_MQTT_Subscribe <feedname> = Adafruit_MQTT_Subscribe(&mqtt, MQTT_NAME "/f/<feedname>");  //subscribing to feed

void setup()
{
  Serial.begin(115200);    //serial communication

  Serial.print("\n\nConnecting Wifi... ");
  WiFi.begin(WIFI_SSID, WIFI_PASS);             //connecting to wifi
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
  }

  Serial.println("OK!");             //if wifi is connected
  mqtt.subscribe(&<feedname>);       //subscribe to feed(channel)

  pinMode(5, OUTPUT);            //GPIO/Digital Output pin 5 of NodeMCU
  digitalWrite(5, HIGH);         //Default state Off

  pinMode(4, OUTPUT);            //GPIO/Digital Output pin 4 of NodeMCU
  digitalWrite(4, HIGH);         //Default state Off

  pinMode(14, OUTPUT);           //GPIO/Digital Output pin 14 of NodeMCU
  digitalWrite(14, HIGH);        //Default state Off
    
  pinMode(12, OUTPUT);           //GPIO/Digital Output pin 12 of NodeMCU
  digitalWrite(12, HIGH);        //Default state Off
}

void loop()
{
  MQTT_connect();           //connect to server

  Adafruit_MQTT_Subscribe * subscription;                     //subscribe to the feed
  while ((subscription = mqtt.readSubscription(5000)))        //check if there is a new value in the feed
  { //if yes
    if (subscription == &<feedname>)         
    {
      Serial.print("<feedname>: ");                            //print the new value
      Serial.println((char*) <feedname>.lastead);             //print the new value

      if (!strcmp((char*) <feedname>.lastread, "<>"))         //if new value is to turn on  switch (ON1)
        digitalWrite(5, LOW);                                 //turn on the switch
      else if (!strcmp((char*) <feedname>.lastread, "<>"))    //if the new value is to turn off the  switch(OFF1)
        digitalWrite(5, HIGH);                                //turn Off the switch

      if (!strcmp((char*) <feedname>.lastread, "<>"))        //if new value is to turn on  switch (ON2)
        digitalWrite(4, LOW);                                //turn on the switch
      else if (!strcmp((char*) <feedname>.lastread, "<>"))   //if new value is to turn off  switch (OFF2)
        digitalWrite(4, HIGH);                               //turn Off the switch

      if (!strcmp((char*) <feedname>.lastread, "<>"))        //if new value is to turn on  switch (ON3)
        digitalWrite(14, LOW);                               //turn on the switch
      else if (!strcmp((char*) <feedname>.lastread, "<>"))   //if new value is to turn off  switch (OFF3)
        digitalWrite(14, HIGH);                              //turn Off the switch
        
       if (!strcmp((char*) <feedname>.lastread, "<>"))       //if new value is to turn on switch (ON3)
        digitalWrite(12, LOW);                               //turn on the switch
      else if (!strcmp((char*) <feedname>.lastread, "<>"))   //if new value is to turn off  switchs (OFF3)
        digitalWrite(12, HIGH);                              //turn Off the switch
    }
  }

  if (!mqtt.ping())         //keep disconnecting and reconnecting to
    mqtt.disconnect();      //make sure that there is a proper connection b\w nodemcu and MQTT server
} 

void MQTT_connect()       //connecting to server
{
  int8_t ret;
  if (mqtt.connected())  //if connected go back to loop
  {
    return;
  }
                         //else continue
  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;                         //3 trials to connect to server then disconnect from wifi and retry
  while ((ret = mqtt.connect()) != 0)
  {
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000);
    retries--;
    if (retries == 0)
    {
      while (1);
    }
  }
  Serial.println("MQTT Connected!");
}
