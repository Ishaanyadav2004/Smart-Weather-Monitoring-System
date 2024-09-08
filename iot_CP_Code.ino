#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPL32MYZF3bh"
#define BLYNK_TEMPLATE_NAME "Weather Station 1"
#include "DHT.h"    
#include <BlynkSimpleEsp8266.h>    
#define DHTTYPE DHT11  
#include <Adafruit_BMP085.h> 
#include <ESP8266WiFi.h>
Adafruit_BMP085 bmp;
#define dht_dpin D6
DHT dht(dht_dpin, DHTTYPE); 
#define VIN 3.3 
#define R 10000 

const int Analog_Pin = 0; 

#define RAIN_SENSOR  3
int LDR_Val;
int Iluminance;
char auth[] = "akJ7wItlHIq18B1K0NDbW5uVkdfYDf4G";//Enter your Auth token
char ssid[] = "mummy";//Enter your WIFI name
char pass[] = "nishayadav4504";//Enter your WIFI password

int flag=0;
BlynkTimer timer;
void notifyOnFire()
{
  /*int isButtonPressed = digitalRead(RAIN_SENSOR);
 
  if (isButtonPressed==0 && flag==0) {
    Serial.println("Its Raining");
     //Blynk.email("shameer50@gmail.com", "Alert", "It's Raining");
     Blynk.logEvent("rain_alert","It's Raining");
    flag=1;
    tone(14,494,500);
    digitalWrite(D7,HIGH);
    delay(1000);
  }
  else if (isButtonPressed==1)
  {
    flag=0;
     tone(14,494,500);
     digitalWrite(D7,LOW);
  }
}*/


void setup(void)
{ 
  dht.begin();
  Serial.begin(9600);
 pinMode(D7,OUTPUT);
  Serial.println("Humidity and temperature \n\n");
   Serial.begin(9600);
 pinMode(RAIN_SENSOR, INPUT);
   Blynk.begin(auth, ssid, pass);
  //dht.begin();
  timer.setInterval(2500L, notifyOnFire);
    delay(700);
    Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
   Serial.begin(9600);
  if (!bmp.begin()) {
      Serial.println("Could not find a valid BMP085 sensor, check wiring!");
      while (1) {}
  }
  //timer.setInterval(100L, DHT11sensor);
  //timer.setInterval(100L, rainSensor);
  //timer.setInterval(100L, pressure);
  //timer.setInterval(100L, LDRsensor);
}

void loop() {
   Blynk.run();//Run the Blynk library
  timer.run();
  //rainValue = digitalRead(raindropSensorPin);

  //Rain drop module 
   int sensorValue = digitalRead(RAIN_SENSOR);
  Serial.print("Rain sensor value: ");
  Serial.println(sensorValue);

  if (sensorValue < RAIN_SENSOR_THRESHOLD) {
    Blynk.logEvent("rain_alert");
  }
    int P,A;
    float h = dht.readHumidity();
    float t = dht.readTemperature();         
    Serial.print("Current humidity = ");
    Serial.print(h);
    Serial.print("%  ");
    Serial.print("temperature = ");
    Serial.print(t); 
    Serial.println("C  ");
    Blynk.virtualWrite(V0, t);
  Blynk.virtualWrite(V1, h);
    //BMP 180 code
   Serial.print("Pressure = ");
    Serial.print(bmp.readPressure());
    P=bmp.readPressure();
    Blynk.virtualWrite(V4,P);
    A=bmp.readAltitude();
    Blynk.virtualWrite(V3,A);
    Serial.println(" Pa");
    Serial.print("Altitude = ");
    Serial.print(bmp.readAltitude());
    Serial.println(" meters");
    Serial.print("Pressure at sealevel (calculated) = ");
    Serial.print(bmp.readSealevelPressure());
    Serial.println(" Pa");
    Serial.print("Real altitude = ");
    Serial.print(bmp.readAltitude(101500));
    Serial.println(" meters");
    Serial.println();
    
   
    LDR_Val = analogRead(Analog_Pin);
  Iluminance = conversion(LDR_Val);
  Serial.print("Analog reading from   LDR sensor = "); 
  Serial.println(LDR_Val);  
  Serial.print("Iluminance value = ");
  Serial.print(Iluminance); 
  Serial.println(" Lux"); 

//Blynk.virtualWrite(V4, Iluminance);
    delay(2000);
} 

int conversion(int raw_val)
  {
  float Vout = float(raw_val) * (VIN / float(1023));
  float RLDR = (R * (VIN - Vout))/Vout; 
  int lux = 500/(RLDR/1000); 
  return lux;
}






