
// Variable Blynk
#define BLYNK_TEMPLATE_ID "TMPL6Xbei2UtW"
#define BLYNK_TEMPLATE_NAME "Smart Mini Conveyor"
#define BLYNK_AUTH_TOKEN "KKKqcs64bb846qX11FxHYBNKRccrwP3b"
#define BLYNK_PRINT Serial

// Variabel Wifi
const char* ssid = "vivo Y33T";
const char* pass = "12345678";

// variabel L298n
const int in1 = 12;
const int in2 = 14;
const int en = 27;

// variabel ultrasonik
const int echoPin = 32;
const int trigPin = 33;
float duration,distance;

// variabel counting
int itemA = 0;
int itemB = 0;

bool itemDetectedA = false;
bool itemDetectedB = false;

// Library
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <ESP32Servo.h>

// object servo
Servo servoA;
Servo servoB;

void setup() {
  // pin mode
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(en, OUTPUT);
  pinMode(trigPin, OUTPUT);  
	pinMode(echoPin, INPUT);

  // servo
  servoA.attach(26);
  servoB.attach(34); 

  Serial.begin(9600);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
}

void loop() {
  Blynk.run();
  digitalWrite(trigPin, LOW);  
	delayMicroseconds(2);  
	digitalWrite(trigPin, HIGH);  
	delayMicroseconds(10);  
	digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH); 
  distance = (duration*.0343)/2;
  Serial.print("Distance: ");  
	Serial.println(distance);  
  // Deteksi untuk variabel A (7-10 cm)
  if (distance < 3) {
    if (!itemDetectedA) {
      itemA++;
      itemDetectedA = true;
      Blynk.virtualWrite(V2, itemA);
      servoA.write(90);
      delay(1000);
      for (int i = 90; i > 0; i--) {
        servoA.write(i);
        delay(20);
      }
    }
  } else {
    itemDetectedA = false;
  }

  // Deteksi untuk variabel B (2-6 cm)
  if (distance > 4 && distance < 8) {
    if (!itemDetectedB) {
      itemB++;
      itemDetectedB = true;
      Blynk.virtualWrite(V3, itemB);
    }
  } else {
    itemDetectedB = false;
  }
  delay(100);
}

BLYNK_WRITE(V0)
{
  int value = param.asInt();
  if (value == 1) {
    digitalWrite(in1,LOW);
    digitalWrite(in2,HIGH);
  }else {
    digitalWrite(in1,LOW);
    digitalWrite(in2,LOW);
  }
  Serial.print("V1 Slider value is: ");
  Serial.println(value);
}

BLYNK_WRITE(V1)
{
  int value = param.asInt();
  analogWrite(en,value);
  Serial.print("V1 Slider value is: ");
  Serial.println(value);
}

BLYNK_WRITE(V4)
{
  int value = param.asInt();
  if (value == 1) {
    itemA = 0;
    itemB = 0;
    Blynk.virtualWrite(V2, itemA);
    Blynk.virtualWrite(V3, itemB);
  }
  Serial.print("V1 Slider value is: ");
  Serial.println(value);
}