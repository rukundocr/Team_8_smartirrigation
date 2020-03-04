#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
char auth[] = "JKNVXMDWhnnBytGzuHY8IPtpNNiE7JUC";
char ssid[] = "ICT Hotspot";
char pass[] = "";
int pin = A0;
float moisture=0;
float temperature=22;
float humidity=70;
BlynkTimer timer;
void sendSensor()
{
 Blynk.virtualWrite(V1, moisture);
 delay (200);
 Blynk.virtualWrite(V2, temperature);
 delay (200);
 Blynk.virtualWrite(V3, humidity);
}
void setup()
{
 // Debug console
 Serial.begin(9600);

 Blynk.begin(auth, ssid, pass);
 // You can also specify server:
 //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 80);
 //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);
 timer.setInterval(1000L, sendSensor);
}
void loop()
{
 Blynk.run();
 timer.run();
 moisture = analogRead(pin);
 Serial.print ("moisture: "); // output to the serial port
 Serial.println (moisture);
 Serial.print ("temperature: "); // output to the serial port
 Serial.println (temperature);
 Serial.print ("humidity: "); // output to the serial port
 Serial.println (humidity);
 delay(2000);
}
