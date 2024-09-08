//#define BLYNK_TEMPLATE_ID "TMPL3TcdyXgmW"
//#define BLYNK_TEMPLATE_NAME "weather monitoring systemCopy"
//#define BLYNK_PRINT Serial
//#include <BlynkSimpleEsp8266.h>  
#include <ESP8266WiFi.h>
#include <ThingSpeak.h>
#define SENSOR_PIN A0

const int sampleWindow = 50; // Sample window width in mS (50 mS = 20Hz)
unsigned int sample;
//#include <BlynkSimpleEsp8266.h>  
/*#include <ESP8266WiFi.h>
char auth[] = "GgUGUB8kRkfa-3knfSbuIGIHIVy-QU33";//Enter your Auth token
char ssid[] = "Ish";//Enter your WIFI name
char pass[] = "ishaanop";//Enter your WIFI password*/
WiFiClient client;
long myChannelNumber =  2502493;
const char myWriteAPIKey[] = "TW0A3LNVIVX3J724";
void setup ()
{ 
  Serial.begin(9600);
   //Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
   WiFi.begin("Ish","ishaanop");
while(WiFi.status() !=WL_CONNECTED)
{delay(100);
Serial.print(".");
}
Serial.println();
Serial.println("NodeMCU is Connected");
Serial.println(WiFi.localIP());
ThingSpeak.begin(client);
}


void loop ()
{
 // Blynk.run();//Run the Blynk library
  
  unsigned long startMillis = millis(); // Start of sample window
  float peakToPeak = 0; // peak-to-peak level

  unsigned int signalMax = 0; //minimum value
  unsigned int signalMin = 1024; //maximum value

  // collect data for 50 mS
  while (millis() - startMillis < sampleWindow)
  {
    sample = analogRead(SENSOR_PIN); //get reading from microphone
    if (sample > signalMax)
    {
      signalMax = sample; // save just the max levels
    }
    else if (sample < signalMin)
    {
      signalMin = sample; // save just the min levels
    }
  }

  peakToPeak = signalMax - signalMin; // max - min = peak-peak amplitude
  int db = map(peakToPeak, 0, 1023, 49.5, 90); //calibrate for deciBels

  Serial.print("Loudness: ");
  Serial.print(db);
  Serial.println("dB");
  //Blynk.virtualWrite(V0, db);
    ThingSpeak.writeField(myChannelNumber, 1, db+30, myWriteAPIKey);
  if (db <= 60)
  {
    Serial.println("Level: Quiet");
  }
  else if (db > 60 && db < 85)
  {
    Serial.println("Level: Moderate");
  }
  else if (db >= 85)
  {
    Serial.println("Level: High");
  }
  delay(2000);
}
