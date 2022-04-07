#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include <DHT.h>
#include <Servo.h>
#include "filtersampah.h"

// Waktu pembacaan sensor
#define waktu 5000

// Persentase Kelembaban dan Maksimum Kapasitas sampah 
#define kelembaban_sampah 85 
#define maks_tempat 32 //cm
#define jarak_buka_otomatis 15 //cm


// pinMode HC-SR 04
#define trig_1 1
#define echo_1 2
#define trig_2 6
#define echo_2 5
#define trig_3 12
#define echo_3 13
#define dht_pin 4

// pinMode Servo
#define pin_servo_1 9
#define pin_servo_2 10
#define pin_servo_3 11

// Pin komunikasi serial
#define rx 7
#define tx 8

// Infrared Obstacle Avoidance PinMode
#define obstaclePin 3
int hasObstacle = HIGH;

SoftwareSerial esp01(rx, tx);
Ultrasonic_kapasitas buka_otomatis(trig_1, echo_1);
Ultrasonic_kapasitas kapasitas_1(trig_2, echo_2);
Ultrasonic_kapasitas kapasitas_2(trig_3, echo_3);
DHT dht(dht_pin, DHT11);


void upload(){
  DynamicJsonBuffer jsonBuffer;
  JsonObject& data = jsonBuffer.createObject();
  

  data["tongsampah_1"] = kapasitas_1.Check(maks_tempat);
  data["tongsampah_2"] = kapasitas_2.Check(maks_tempat);
  //data["tongsampah_3"] = "100";

  data.printTo(esp01);
  jsonBuffer.clear();

  delay(500);
}

int pemilah_ano_organik(){
  int h = dht.readHumidity();
  if(isnan(h)){ 
    return;
  }

  return h;
}

void setup() {
  
  Serial.begin(9600);
  esp01.begin(115200);
  dht.begin();
  
  kapasitas_1.begin();
  kapasitas_2.begin();

  servo_initializer(pin_servo_1, pin_servo_2, pin_servo_3);
  gerak_standby();

  pinMode(obstaclePin, INPUT);
}

void loop() {
  if(buka_otomatis.distance() >= jarak_buka_otomatis){
    gerak_buka();
    upload();
    hasObstacle = digitalRead(obstaclePin);
    Serial.println(pemilah_ano_organik());
    
    if(hasObstacle == LOW){
      delay(waktu);
      if(pemilah_ano_organik() <= kelembaban_sampah){
        if(kapasitas_1.Check(maks_tempat) >= 80){
          gerak_standby();
          Serial.print(F("Tong sampah Organik sudah penuh "));
          Serial.println(kapasitas_1.Check(maks_tempat));
        } 
        else {
          //Servo 2 ke kanan? or kiri?
          gerak_kanan();
          delay(3000);
          gerak_standby();
          delay(3000);
        }
      } 
      else{
        if(kapasitas_2.Check(maks_tempat) <= 80){
          gerak_standby();
          Serial.print(F("Tong sampah Anorganik sudah penuh "));
          Serial.println(kapasitas_2.Check(maks_tempat));
        } 
        else {
          //Servo 2 ke kanan? or kiri?
          gerak_kiri();
          delay(3000);
          gerak_standby();
          delay(3000);
        }
      }
    } 
    else {
      //TODO
      gerak_standby();
    }
  } 
  else {
    //TODO 
    gerak_tutup();
  }
}
