#include <Arduino.h>
#include <iostream>
#include <string>

HardwareSerial Receiver(0);

std::string buffer = "";//This creates a new variable called "buffer" of type std::string and assigns an empty string to it.

const int freq = 5000;
const int channel1 = 0;
const int channel2 = 1;
const int channel3 = 2;
const int channel4 = 3;
const int resolution = 8;
unsigned long previous_millis = 0;
const int debounce_time = 1000;  // debounce time in milliseconds
bool current_direction = LOW;
int stepsPerRevolution = 200;

#define pump_dir 23
#define pump_pwm 18
#define vaccum 26
#define stepper_step 21
#define stepper_dir 22
#define auger_pwm 27
#define auger_dir 33
#define auger_rot_pwm 32
#define auger_rot_dir 13
#define senser_suit_pwm 16
#define senser_suit_dir 17

char sm = '0';
//void IRAM_ATTR function_ISR() {
//  ets_printf("Limit Switch Triggered");
//  for (int i = 0; i < 5000; i++)
//  {
//    digitalWrite(auger_dir, HIGH);
//    digitalWrite(auger_rot_dir, HIGH);
//    ledcWrite(channel1, 255);
//    ledcWrite(channel2, 255);
//  }
//
//}
void Stop(void) {
  Serial.println("Stop");
  ledcWrite(channel1, 0);
  ledcWrite(channel2, 0);
  ledcWrite(channel3, 0);
  //  digitalWrite(relay1, LOW);
  //  digitalWrite(relay2, LOW);
  //  digitalWrite(relay3, LOW);
  //  digitalWrite(relay4, LOW);
  //  digitalWrite(vaccum, LOW);
  digitalWrite(stepper_step, LOW);
  digitalWrite(auger_dir, LOW);
  digitalWrite(auger_rot_dir, LOW);
  digitalWrite(senser_suit_dir, LOW);

}
void setup()
{

  Serial.begin(115200);
  Receiver.begin(115200); //(baud rate,protocol,Tx,Rx)
//  Receiver.begin(115200, 25, 26); //(baud rate,protocol,Tx,Rx)


  //  pinMode(limit_switch_top, INPUT_PULLUP);
  //  attachInterrupt(limit_switch_top, function_ISR, CHANGE);
  pinMode(vaccum, OUTPUT);          // vaccum
  pinMode(stepper_step, OUTPUT);    // stepper step
  pinMode(stepper_dir, OUTPUT);     // stepper direction
  pinMode(auger_dir, OUTPUT);       // auger actuation direction
  pinMode(auger_rot_dir, OUTPUT);   // auger rotation direction
  pinMode(senser_suit_dir, OUTPUT); // sensor suite direction
  pinMode(pump_dir, OUTPUT);        // pump direction

  ledcSetup(channel1, freq, resolution);
  ledcSetup(channel2, freq, resolution);
  ledcSetup(channel3, freq, resolution);
  ledcSetup(channel4, freq, resolution);

  ledcAttachPin(auger_pwm, channel1);       // auger actuation PWM
  ledcAttachPin(auger_rot_pwm, channel2);   // auger rotation PWM
  ledcAttachPin(senser_suit_pwm, channel3); // sensor suite PWM
  ledcAttachPin(pump_pwm, channel4);        // pump PWM

  //Channel 1 = Auger Up & Down
  //Channel 2 = Auger Rotation
  //Channel 3 = Sensor Suite Up and Down
  //Channel 4 = Pump On & Off

  ledcWrite(channel1, 0);
  ledcWrite(channel2, 0);
  ledcWrite(channel3, 0);
  ledcWrite(channel4, 0);
  digitalWrite(vaccum, LOW);
  digitalWrite(stepper_step, LOW);
  digitalWrite(stepper_dir, LOW);
  digitalWrite(auger_dir, LOW);
  digitalWrite(auger_rot_dir, LOW);
  digitalWrite(senser_suit_dir, LOW);
  digitalWrite(pump_dir, LOW);
}

void loop()
{
  if (Receiver.available()) {


    while (Receiver.available())
    {
      char RxdChar = Receiver.read();
      Serial.println(RxdChar);
      SMControl(RxdChar);
    }

    delay(100);
  }
  else
  {
    Stop();
  }

}
void SMControl(char buffer)
{
  switch (buffer)
  {
    case '0': // safety
      ledcWrite(channel1, 0);
      ledcWrite(channel2, 0);
      ledcWrite(channel3, 0);
      ledcWrite(channel4, 0);
      digitalWrite(vaccum, LOW);
      digitalWrite(stepper_step, LOW);
      //      digitalWrite(stepper_dir, LOW);
      digitalWrite(auger_dir, LOW);
      digitalWrite(auger_rot_dir, LOW);
      digitalWrite(senser_suit_dir, LOW);
      digitalWrite(pump_dir, LOW);
      break;

    case '1': // pump
      Serial.println("pump");
      digitalWrite(pump_dir, LOW);
      ledcWrite(channel4, 200);
      break;

    case '2': // vaccum
      Serial.println("vaccum");
      digitalWrite(vaccum, HIGH);
      break;

    case '3': // stepper rotate
      Serial.println(current_direction);
      for (int x = 0; x < 3; x++)
      {
        digitalWrite(stepper_step, HIGH);
        delayMicroseconds(500);
        digitalWrite(stepper_step, LOW);
        delayMicroseconds(500);
      }
      //      digitalWrite(stepper_step, HIGH);
      //      delayMicroseconds(200);
      //      digitalWrite(stepper_step, LOW);
      //      delayMicroseconds(200);
      break;

    case '4': // stepper direction toggle
    Serial.println("current_direction");
      Serial.println(current_direction);
      if (millis() - previous_millis >= debounce_time) {
        previous_millis = millis();
        if (current_direction == LOW) {
          digitalWrite(stepper_dir, HIGH);
          current_direction = HIGH;
        }
        else {
          digitalWrite(stepper_dir, LOW);
          current_direction = LOW;
        }
      }
      break;

    case '5': // auger down with rotation
      Serial.println("Hey its 500 mo-fkers");
      for (int i = 0; i < 500; i++)
      {
        digitalWrite(auger_dir, LOW);//Changed from HIGH to LOW
        digitalWrite(auger_rot_dir, HIGH);
        ledcWrite(channel1, 255);
        ledcWrite(channel2, 255);
      }
      break;

    case '6': // auger Rotation for Deposition
      for (int i = 0; i < 500; i++)
      {
        digitalWrite(auger_dir, LOW);
        digitalWrite(auger_rot_dir, LOW);
        ledcWrite(channel1, 0);
        ledcWrite(channel2, 255);
      }
      break;

    case '7': // auger rotation for deposition
      for (int i = 0; i < 500; i++)
      {

        digitalWrite(auger_dir, HIGH);
        digitalWrite(auger_rot_dir, LOW);
        ledcWrite(channel1, 255);
        ledcWrite(channel2, 0);
      }
      break;

    case '8': // sensor suite up
      for (int i = 0; i < 500; i++)
      {
        digitalWrite(senser_suit_dir, HIGH);
        ledcWrite(channel3, 255);
      }
      break;

    case '9': // sensor suite down
      for (int i = 0; i < 500; i++)
      {
        digitalWrite(senser_suit_dir, LOW);
        ledcWrite(channel3, 255);
      }
      break;

    case 'A': // ESP Restart
      ESP.restart();
      break;

    default:
      ledcWrite(channel1, 0);
      ledcWrite(channel2, 0);
      ledcWrite(channel3, 0);
      ledcWrite(channel4, 0);
      //      digitalWrite(vaccum, LOW);
      //      digitalWrite(stepper_step, LOW);
      //      digitalWrite(stepper_dir, LOW);
      //      digitalWrite(auger_dir, LOW);
      //      digitalWrite(auger_rot_dir, LOW);
      //      digitalWrite(senser_suit_dir, LOW);
      //      digitalWrite(pump_dir, LOW);
      break;
  }
}
