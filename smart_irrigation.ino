// Robo India Tutorial 
// Simple code upload the tempeature and humidity data using thingspeak.com
// Hardware: NodeMCU,potentiometer

#include <ESP8266WiFi.h>
#include "ThingSpeak.h"
String apiKey = "BBAQJGBK6W11JRY4";     //  Enter your Write API key from ThingSpeak
unsigned long channel =997843;//Replace with your own ThingSpeak Account Channle ID
const char *ssid =  "iphone";     // replace with your wifi ssid and wpa2 key
const char *password =  "08081986";    //REPLACE with your wifi password
const char* server = "api.thingspeak.com";//thingspeak server IP
//1,2 and 3 are channel fields. You don't need to change if you are following this tutorial. However, you can modify it according to your application
unsigned int Water_Pump = 1;
const int analog_ip = A0; //Naming analog input pin
int moisture = 0;        //Variable to store analog input values
int pumpin =D1;
float temp;
float humidity;
int pump_status =0;
WiFiClient client;
 
void setup() 
{
  Serial.begin(115200);
  delay(100);   
  pinMode(pumpin, OUTPUT);
  digitalWrite(pumpin, 0);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
 
      while (WiFi.status() != WL_CONNECTED) 
     {
      delay(500);
      Serial.print(".");//print dot while waiting coonction to wifi
     }
     // when wifi connection is suceesssfully connected 
   Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Netmask: ");
  Serial.println(WiFi.subnetMask());
  Serial.print("Gateway: ");
  Serial.println(WiFi.gatewayIP());
  ThingSpeak.begin(client);
}
 
void loop() 
{
 //get the last data of the fields
int pump = ThingSpeak.readFloatField(channel, Water_Pump);
if(pump == 1){                                               // turn ON motor if thingspeak pump field = 1
    //digitalWrite(pumpin, HIGH);
    Serial.println("WATER PUMP IS  is On..!");
  }
  else if(pump == 0){                                      //turn OFF motor if thingspeak pump field = 1
    //digitalWrite(pumpin, LOW);
    Serial.println("WATER PUMP  is Off..!");
  }
  
  temp= 50;
  humidity=100;
  moisture = analogRead(analog_ip); // Analog Values 0 to 1023
  if(moisture > 650){
    digitalWrite(pumpin, HIGH);
    Serial.println("WATER PUMP IS  is On..!");
    pump_status=1;
    }else{
       digitalWrite(pumpin, LOW);
    Serial.println("WATER PUMP  is Off..!");
    pump_status=0;
      }
  Serial.println("moisture content of the soil:");
  Serial.println (moisture);
   Serial.println("TEMPERATURE:");
  Serial.println (temp);
  Serial.println("Humidity:");
  Serial.println (humidity);
  

                         if (client.connect(server,80))   //   "184.106.153.149" or api.thingspeak.com
                      {  
                            
                             String postStr = apiKey;
                             postStr +="&field1=";
                             postStr += String(moisture);
                             postStr +="&field2=";
                             postStr += String(temp);
                             postStr +="&field3=";
                             postStr += String(humidity);
                             postStr +="&field4=";
                             postStr += String(pump_status);
                             postStr += "\r\n\r\n";
                              
 
                             client.print("POST /update HTTP/1.1\n");
                             client.print("Host: 184.106.153.149\n");
                             client.print("Connection: close\n");
                             client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
                             client.print("Content-Type: application/x-www-form-urlencoded\n");
                             client.print("Content-Length: ");
                             client.print(postStr.length());
                             client.print("\n\n");
                             client.print(postStr);
 
                             Serial.print("Temperature: ");
                             Serial.println("%. Send to Thingspeak.");
                        }
          client.stop();
 
          Serial.println("Waiting...");
  
  // thingspeak needs minimum 15 sec delay between updates, i've set it to 30 seconds
  delay(10000);
}
