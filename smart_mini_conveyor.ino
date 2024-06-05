
// Variable Blynk
#define BLYNK_TEMPLATE_ID "TMPL6Xbei2UtW"
#define BLYNK_TEMPLATE_NAME "Smart Mini Conveyor"
#define BLYNK_AUTH_TOKEN "KKKqcs64bb846qX11FxHYBNKRccrwP3b"
#define BLYNK_PRINT Serial

// Variabel Wifi
const char* ssid = "vivo Y33T";
const char* pass = "12345678";

// variabel L298n
const int in1 = 14;
const int in2 = 12;
const int en = 13;

// variabel ultrasonik
const int echoPin = 27;
const int trigPin = 26;
float duration,distance;

// variabel counting
int itemA = 0;
int itemB = 0;
int itemC = 0;
int total = 0;

// variabel untuk mendeteksi barang
bool itemDetectedA = false;
bool itemDetectedB = false;
bool itemDetectedC = false;


// Library
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <ESP32Servo.h>

// object servo
Servo servoA;
Servo servoB;

// Blynk Timer
BlynkTimer timer;

void setup() {
  // pin mode
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(en, OUTPUT);
  pinMode(trigPin, OUTPUT);  
	pinMode(echoPin, INPUT);

  // servo
  servoA.attach(25);
  servoB.attach(33);
  servoA.write(0);
  servoB.write(90);


  Serial.begin(9600);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Setup a function to be called every 100ms
  timer.setInterval(100L, sortir);
}

void sortir() {
  digitalWrite(trigPin, LOW);  
	delayMicroseconds(2);  
	digitalWrite(trigPin, HIGH);  
	delayMicroseconds(10);  
	digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH); 
  distance = (duration*.0343)/2;
  Serial.print("Distance: ");  
	Serial.println(distance);

  // Deteksi untuk variabel A (6,27-10,27 cm)
  if (distance <= 4) {
    if (!itemDetectedA) {
      itemA++;
      total++;
      itemDetectedA = true;
      Blynk.virtualWrite(V2, itemA);
      Blynk.virtualWrite(V6, total);
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

  // Deteksi untuk variabel B (1,37-6,26 cm)
  if (distance > 4 && distance < 9) {
    if (!itemDetectedB) {
      itemB++;
      total++;
      itemDetectedB = true;
      Blynk.virtualWrite(V3, itemB);
      Blynk.virtualWrite(V6, total);
      servoB.write(0);
      delay(1000);
      for (int i = 0; i < 90; i++) {
        servoB.write(i);
        delay(20);
      }
    }
  } else {
    itemDetectedB = false;
  }

  //  Deteksi untuk variabel C (0,37-1,36 cm)
  if (distance >= 9 && distance <= 9.90) {
    if (!itemDetectedB) {
      itemC++;
      total++;
      itemDetectedC = true;
      Blynk.virtualWrite(V5, itemC);
      Blynk.virtualWrite(V6, total);
      delay(1180);
    }
  } else {
    itemDetectedC = false;
  }
}

void loop() {
  Blynk.run();
  timer.run();
}

BLYNK_WRITE(V0)
{
  int value = param.asInt();
  if (value == 1) {
    digitalWrite(in1,HIGH);
    digitalWrite(in2,LOW);
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
    itemC = 0;
    total = 0;
    Blynk.virtualWrite(V2, itemA);
    Blynk.virtualWrite(V3, itemB);
    Blynk.virtualWrite(V5, itemC);
    Blynk.virtualWrite(V6, total);
  }
  Serial.print("V1 Slider value is: ");
  Serial.println(value);
}