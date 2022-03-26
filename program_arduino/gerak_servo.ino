/*Derajat Servo*/
// Standby 90 derajat
#define servo_standby_1 95
#define servo_standby_2 85

// Servo 1
#define servo_kiri_1 0
#define servo_kanan_1 180

// Servo 2
#define servo_kiri_2 180
#define servo_kanan_2 0

//Jarak buka oto
#define autoopen 40

Servo servo_1;
Servo servo_2;

void servo_initializer(const int S, const int R){
  servo_1.attach(S);
  servo_2.attach(R);
}

void gerak_standby(){
  servo_1.write(servo_standby_1);
  servo_2.write(servo_standby_2);
  //delay(1000);
}

void gerak_kanan(){
  servo_1.write(servo_kanan_1);
  servo_2.write(servo_kanan_2);
  delay(1000);
}

void gerak_kiri(){
  servo_1.write(servo_kiri_1);
  servo_2.write(servo_kiri_2);
  delay(1000);
}
