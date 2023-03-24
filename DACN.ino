#include <ESP8266WiFi.h>
#include <SimpleTimer.h>
#define BLYNK_TEMPLATE_ID "TMPL6Ul61o0o"
#define BLYNK_DEVICE_NAME "DS18B20"
#define BLYNK_FIRMWARE_VERSION        "0.1.0"
#define BLYNK_PRINT Serial
#define SensorPin A0            
#define Offset 0.70            
unsigned long int avgValue; 
#include <OneWire.h> 
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 5 
OneWire oneWire(ONE_WIRE_BUS); 
DallasTemperature cambien(&oneWire);
#define APP_DEBUG
#define USE_NODE_MCU_BOARD
#include "BlynkEdgent.h"
#include "DHT.h"            
const int DHTPIN = 4;   
const int DHTTYPE = DHT11;  
DHT dht(DHTPIN, DHTTYPE);
String apiKey = "53I4CTKRHYUGRN85"; 
const char* ssid = "ABC";
const char* password = "12345678";
const char* sever = "api.thingspeak.com";
WiFiClient client;
const int TRIG_PIN  = 12; 
const int ECHO_PIN  = 13; 
const int RELAY_PIN = 14; 
float Muc_nuoc_cach = 4.5; 
float thoigian_us, khoangcach_cm;
WidgetLED StatusD5(V6);
WidgetLED led(V0);
BlynkTimer timer;
void blinkLedWidget(){
 if (led.getValue()) {
    led.off();
  } else {
    led.on();
  }
}

BLYNK_WRITE(V6)
{
 int pinValue = param.asInt(); 
 digitalWrite(D5,pinValue);
 Blynk.virtualWrite(V6, pinValue);
 if (!digitalRead(D5)) 
   {
    StatusD5.off(); 
     Blynk.virtualWrite(V7, "TẮT BƠM");
     delay(200);
   }
   else 
   {
    StatusD5.on();
      Blynk.virtualWrite(V7, "MỞ BƠM");
      delay(200);
   }
}
void setup(){
 Serial.begin(115200);
 delay(100);
 BlynkEdgent.begin();
 cambien.begin();
 dht.begin(); 
 Serial.println("Ready");    
 WiFi.begin(ssid, password);
 timer.setInterval(1000, blinkLedWidget);
 pinMode(TRIG_PIN, OUTPUT);  
 pinMode(ECHO_PIN, INPUT);   
 pinMode(RELAY_PIN, OUTPUT); 
 pinMode(D5,OUTPUT);
 digitalWrite(D5,LOW);
    }
void loop() {
  BlynkEdgent.run();
  timer.run();
 cambien.requestTemperatures();
 Serial.print("Nhiệt độ nước: "); 
 Serial.print(cambien.getTempCByIndex(0));// có nhiều IC trên một bus ko nhất thiết chỉ một IC trên 1 dây
 delay(200);
 
 if(float(cambien.getTempCByIndex(0)) > 30)
 Blynk.virtualWrite(V8, "Nhiệt độ tăng");
 else if (float(cambien.getTempCByIndex(0)) < 22)
 Blynk.virtualWrite(V8, "Nhiệt độ thấp"); 
 else
 Blynk.virtualWrite(V8, "Nhiệt độ phù hợp"); 
 
 Blynk.virtualWrite(V1,cambien.getTempCByIndex(0));
 Serial.print("\t");
 digitalWrite(TRIG_PIN, HIGH);
 delayMicroseconds(10);
 digitalWrite(TRIG_PIN, LOW);
 thoigian_us = pulseIn(ECHO_PIN, HIGH);
 khoangcach_cm = 0.017 * thoigian_us;
 if(khoangcach_cm < Muc_nuoc_cach)
 { digitalWrite(RELAY_PIN, LOW);
 Serial.println("TẮT BƠM");
 StatusD5.off(); 
 delay(200);
 Blynk.virtualWrite(V7, "TẮT BƠM");
   }
 else
    {
 digitalWrite(RELAY_PIN, HIGH); 
 Serial.println("MỞ BƠM");
 StatusD5.on();
 delay(200);
 Blynk.virtualWrite(V7, "MỞ BƠM");
 }
 Serial.print("Khoảng cách mực nước: ");
 Serial.print(khoangcach_cm);
 Serial.println(" cm");
 Blynk.virtualWrite(V5,khoangcach_cm);
 delay(3000);
 int buf[10];           
 for(int i=0;i<10;i++)       
 { 
 buf[i]=analogRead(SensorPin);
 delay(10);
 }
 for(int i=0;i<9;i++)        
 {
 for(int j=i+1;j<10;j++)
 {
 if(buf[i]>buf[j])
 {
 int temp=buf[i];
 buf[i]=buf[j];
 buf[j]=temp;
 }
 }
 }
 avgValue=0;
 for(int i=2;i<8;i++)                     
 avgValue+=buf[i];
 float phValue=(float)avgValue*5.0/1024/6; 
 phValue=2.0*phValue+Offset;                      
 Serial.print("Độ pH: ");  
 Serial.print(phValue,2);
 delay(200);
 if(phValue > 8.5)
 Blynk.virtualWrite(V9, "PH CAO");
 else if(phValue < 5.5)
 Blynk.virtualWrite(V9, "PH THẤP");
 else
 Blynk.virtualWrite(V9, "PH PHÙ HỢP");
 Blynk.virtualWrite(V2,phValue);
float h = dht.readHumidity();    
float t = dht.readTemperature(); 
 Serial.print("Nhiệt độ môi trường: ");
 Serial.print(t);
 Serial.print("°C"); 
 delay(200);
 Serial.print("\t");            
 Serial.print("Độ ẩm: ");
 Serial.print(h);  
 Serial.println("%"); 
 delay(200);                                      
 Serial.println("Send to Thingspeak...");
 Blynk.virtualWrite(V3,t);
 Blynk.virtualWrite(V4,h);
 if (client.connect(sever,80))  {   
 String postStr = apiKey;
 postStr +="&field1=";
 postStr += String(phValue);
 postStr += "\r\n";
 postStr +="&field2="; 
 postStr += String(cambien.getTempCByIndex(0));
 postStr += "\r\n";
 postStr +="&field3=";
 postStr += String(t);
 postStr += "\r\n";
 postStr +="&field4=";
 postStr += String(h);
 postStr += "\r\n";
 client.print("POST /update HTTP/1.1\n");
 client.print("Host: api.thingspeak.com\n");
 client.print("Connection: close\n");
 client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
 client.print("Content-Type: application/x-www-form-urlencoded\n");
 client.print("Content-Length: ");
 client.print(postStr.length());
 client.print("\n\n");
 client.print(postStr);
 }
 client.stop();
 Serial.println("Waiting...");
 }
