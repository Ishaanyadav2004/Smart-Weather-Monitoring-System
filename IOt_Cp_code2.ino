
#define BLYNK_TEMPLATE_ID "TMPL3TcdyXgmW"
#define BLYNK_TEMPLATE_NAME "weather monitoring systemCopy"
#define BLYNK_PRINT Serial
#define SENSOR_PIN D6 // Define the digital pin to which the sound sensor is connected
#include <BlynkSimpleEsp8266.h>  
#include <ESP8266WiFi.h>
#define GasSensorPin A0
const int sampleWindow = 50; // Sample window width in mS (50 mS = 20Hz)
unsigned int sample;

char auth[] = "GgUGUB8kRkfa-3knfSbuIGIHIVy-QU33"; // Enter your Auth token
char ssid[] = "A 31"; // Enter your WIFI name
char pass[] = "vipra2004"; // Enter your WIFI password

void setup() {
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
 pinMode(D7,OUTPUT);
}

void loop() {
 Blynk.run(); // Run the Blynk library
  
  unsigned long startMillis = millis(); // Start of sample window
  float peakToPeak = 0; // peak-to-peak level

  unsigned int signalMax = 0; //minimum value
  unsigned int signalMin = 1; //maximum value

  // collect data for 50 mS
  while (millis() - startMillis < sampleWindow) {
    sample = digitalRead(SENSOR_PIN); // Read digital input from the sensor
    if (sample == HIGH) {
      signalMax = HIGH; // If sound is detected, set the max signal value to HIGH
    }
    else {
      signalMin = LOW; // If no sound is detected, set the min signal value to LOW
    }
  }

  peakToPeak = signalMax - signalMin; // max - min = peak-peak amplitude
   int db = map(peakToPeak, 0, 1, 49.5, 90); //calibrate for deciBels (not needed for digital pin)

  Serial.print("Sound detected: ");
  Serial.print(db);
  Serial.println(peakToPeak ? "Yes" : "No"); // Print if sound is detected or not

  Blynk.virtualWrite(V0, peakToPeak ? 1 : 0);
   if(db == 49) {
    WidgetLED led1(V0);
     led1.off();
     digitalWrite(D7,LOW);
//If the value is 1, the LED widget is dimmed
  } else if (db == 90) {
    WidgetLED led1(V0);
    led1.on();
     tone(14,1000,500);
  digitalWrite(D7,HIGH);
  Blynk.logEvent("sound_detection","Loud Sound");
  }
  int gasValue = analogRead(GasSensorPin);

  int gasThreshold = 350; 

  Serial.print("Gas Value: ");
  Serial.println(gasValue);

  if (gasValue > gasThreshold) {
      WidgetLED led2(V1);
     led2.on();
    Serial.println("Gas detected! Buzzer is ON");
    tone(14,1050,500);
   digitalWrite(D7,HIGH);
   Blynk.logEvent("gas_detection","Gas Leakage (Gas Detected)");
  } else {
     WidgetLED led2(V1);
     led2.off();
    Serial.println("No gas detected. Buzzer is OFF");
   digitalWrite(D7,LOW);
  }
  delay(1000);
}

