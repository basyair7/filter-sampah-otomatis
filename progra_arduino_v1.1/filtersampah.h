#include <Servo.h>

class Ultrasonic_kapasitas{
  private:
    uint8_t trig_pin;
    uint8_t echo_pin;
    float persen, diff;
    long duration, _distance;
    
  public:
    Ultrasonic_kapasitas(uint8_t trig, uint8_t echo){
      trig_pin = trig;
      echo_pin = echo;
    }
    
    void begin(){
      pinMode(trig_pin, OUTPUT);
      pinMode(echo_pin, INPUT);
      
    }
    
    float Check(uint16_t max){
      // Transmitting pulse
      digitalWrite(trig_pin, LOW);
      delayMicroseconds(2);
      digitalWrite(trig_pin, HIGH);
      delayMicroseconds(10);
      digitalWrite(trig_pin, LOW);
      duration = pulseIn(echo_pin, HIGH);
      _distance = (duration/2) / 29.1;
      diff = max - _distance;

      /* 'diff' variable tells u that how much the
       * Garbage Bin is Left to fill 
       */

      return persen = (diff/max)*100;

      /* 'perc variable tells u that, how much percentage the
       * Garbage Bin is filled.
       */
    }

    float distance(){
      // Transmitting pulse
      digitalWrite(trig_pin, LOW);
      delayMicroseconds(2);
      digitalWrite(trig_pin, HIGH);
      delayMicroseconds(10);
      digitalWrite(trig_pin, LOW);
      duration = pulseIn(echo_pin, HIGH);
      
      return _distance = (duration/2) / 29.1;
    }
};
