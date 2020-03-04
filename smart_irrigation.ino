// Robo India Tutorial 
// Simple code upload the tempeature and humidity data using thingspeak.com
// Hardware: NodeMCU,potentiometer
#define BLYNK_PRINT Serial
#include <BlynkSimpleEsp8266.h>
char auth[] = "JKNVXMDWhnnBytGzuHY8IPtpNNiE7JUC"; //auth code from blynk 
#include <ESP8266WiFi.h>
#include "ThingSpeak.h"  // thingspeak lib
#include "DHT.h"   // include dht library
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
#define DHTPIN 2     // Digital pin connected to the DHT sensor
DHT dht(DHTPIN, DHTTYPE);
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
BlynkTimer timer;
WiFiClient client;
 void sendSensor()
{
 Blynk.virtualWrite(V1, moisture);// virtual pin 1 will display moisture
 delay (200);
 Blynk.virtualWrite(V2, temperature);//virtual pin2 will display temperature
 delay (200);
 Blynk.virtualWrite(V3, humidity);// virtual pin 3 will display humidity
}
void setup() 
{
  //----start blink
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(1000L, sendSensor);
  //---end blind--------
  Serial.begin(115200);
  dht.begin();
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
  
 Blynk.run();
 timer.run();
  
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
  
// --------------------- dht codes 
float humidity = dht.readHumidity();
// Read temperature as Celsius (the default)
float temperature = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

//----------------------end dht code----------------

//------------moisture sensor reading-------------- 
  moisture = analogRead(analog_ip); // Analog Values 0 to 1023
 //------------end moisture sensor reading------------------

 //----watering process depending on the moisture sensor reading------------
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
 // -----------end watering code -------------------

 //---------------  thingspeak code to send data to the thingspeak server 

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
  //--------------end sending to thingspeak-----------------------------
  // thingspeak needs minimum 15 sec delay between updates, i've set it to 30 seconds
  delay(10000);
}
