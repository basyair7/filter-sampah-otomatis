#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include <FirebaseESP8266.h>

// Pin Komunikasi Serial
#define rx D6 // to pin arduino 8
#define tx D7 // to pin arduino 7

// Setting find WiFi Hotspot
#define WIFI_SSID "@Wifi.com"
#define WIFI_PASSWORD "Hotspot_ahul07"

// Setting Firebase database
#define FIREBASE_HOST "https://smart-trash-17292-default-rtdb.asia-southeast1.firebasedatabase.app/"
#define FIREBASE_AUTH "jFwHLTHsMtkNksCO9e6TLYoAT8dhsY9zsJiCspxb"

// root json firebase
const String path_1 = "data_sensor/tongsampah_1";
const String path_2 = "data_sensor/tongsampah_2";
const String path_3 = "data_sensor/tongsampah_3";


FirebaseData firebaseData; //firebase function
SoftwareSerial arduino_board(rx, tx);

//void ICACHE_RAM_ATTR loop(); // jika nodemcu intrrupt

void setup() {
  Serial.begin(115200);
  
  // setup wifi connection
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  //Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    //Serial.print(".");
    delay(500);
  }
//  Serial.println();
//  Serial.println("Connected.");
    
  // Setup firebase auth & host link
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  //Firebase.reconnectWiFi(true);
  //firebaseData.setBSSLBufferSize(1024, 1024);
  //firebaseData.setResponseSize(1024);
  Firebase.setReadTimeout(firebaseData, 1000 * 60);
  //Firebase.setwriteSizeLimit(firebaseData, "tiny");
  
  arduino_board.begin(115200);
  while (!Serial) continue;
  
}

void loop() {
  // get data from arduino
  DynamicJsonBuffer jsonBuffer;
  JsonObject& data = jsonBuffer.parseObject(arduino_board);
  if (data == JsonObject::invalid()){
    jsonBuffer.clear();
    return;
  }

  String getdata_1 = data["tongsampah_1"];
  String getdata_2 = data["tongsampah_2"];
  String getdata_3 = data["tongsampah_3"];
  
  // Push data in database
  Firebase.setString(firebaseData, path_1, getdata_1);
  delay(5);
  Firebase.setString(firebaseData, path_2, getdata_2);
  delay(5);
  Firebase.setString(firebaseData, path_3, getdata_3);
  
  //jsonBuffer.clear();
  delay(15);
}
