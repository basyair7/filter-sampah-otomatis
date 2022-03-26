#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include <NewPing.h>
#include <DHT.h>
#include <Servo.h>


/*Kapasitas Sampah*/
#define kapasitas_100 3 //cm
#define kapasitas_80 8
#define kapasitas_70 11
#define kapasitas_50 13
#define kapasitas_30 15
#define kapasitas_10 18
#define kapasitas_0 20

//waktu pembacaan sensor
const int waktu = 5000;

//Persentase Kelembaban sampah
#define kelembaban_sampah 85


// pinMode Module
//#define trig_1 2
//#define echo_1 3
#define trig_2 12
#define echo_2 13
#define trig_3 6
#define echo_3 5
#define dht_pin 9
#define pin_servo_1 10
#define pin_servo_2 11


//Infrared Obstacle Avoidance PinMode
#define obstaclePin 3
int hasObstacle = HIGH;

//Max distance ultrasonic
#define max_distance 200

SoftwareSerial esp01(7, 8);
//NewPing sonar1(trig_1, echo_1, max_distance);
NewPing sonar2(trig_2, echo_2, max_distance);
NewPing sonar3(trig_3, echo_3, max_distance);
DHT dht(dht_pin, DHT11);


/*
int jarak(){
  delay(50);
  return sonar1.ping_cm();
}
*/

int kapasitas_1(){
  if(sonar2.ping_cm() <= kapasitas_100){
    return 100;
  }
  else if(sonar2.ping_cm() <= kapasitas_80){
    return 80;
  }
  else if(sonar2.ping_cm() <= kapasitas_70){
    return 70;
  }
  else if(sonar2.ping_cm() <= kapasitas_50){
    return 50;
  }
  else if(sonar2.ping_cm() <= kapasitas_30){
    return 30;
  }
  else if(sonar2.ping_cm() <= kapasitas_10){
    return 10;
  }
  else if(sonar2.ping_cm() <= kapasitas_0){
    return 0;
  }
  
  delay(50);
}

int kapasitas_2(){
  delay(50);
  if(sonar3.ping_cm() <= kapasitas_100){
    return 100;
  }
  else if(sonar3.ping_cm() <= kapasitas_80){
    return 80;
  }
  else if(sonar3.ping_cm() <= kapasitas_70){
    return 70;
  }
  else if(sonar3.ping_cm() <= kapasitas_50){
    return 50;
  }
  else if(sonar3.ping_cm() <= kapasitas_30){
    return 30;
  }
  else if(sonar3.ping_cm() <= kapasitas_10){
    return 10;
  }
  else if(sonar3.ping_cm() <= kapasitas_0){
    return 0;
  }
  
  delay(50);
}


void upload(){
  DynamicJsonBuffer jsonBuffer;
  JsonObject& data = jsonBuffer.createObject();
  

  data["tongsampah_1"] = kapasitas_1();
  data["tongsampah_2"] = kapasitas_2();
  //data["tongsampah_3"] = "100";

  data.printTo(esp01);
  jsonBuffer.clear();

  delay(500);
}

float pemilah_ano_organik(){
  float h = dht.readHumidity();
  if(isnan(h)){ 
    return;
  }

  return h;
}

void setup() {
  
  Serial.begin(9600);
  esp01.begin(115200);
  dht.begin();
  
  servo_initializer(pin_servo_1, pin_servo_2);
  gerak_standby();

  pinMode(obstaclePin, INPUT);
}

void loop() {
  upload();
  hasObstacle = digitalRead(obstaclePin);
  
  if(hasObstacle == LOW){
    delay(waktu);
    Serial.println(pemilah_ano_organik());
    if(pemilah_ano_organik() <= kelembaban_sampah){
      if(kapasitas_1() == 100){
        gerak_standby();
    
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
      if(kapasitas_2() == 100){
        gerak_standby();
        delay(3000);
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
