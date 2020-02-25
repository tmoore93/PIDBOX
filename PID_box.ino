const int motor_pin = A0;
const int photo_pin = 2;
const int pot_pin = A1;
const int swtch_pin = 3;
const int iteration_time = 10;
const int scl = 1;
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3F,16,2);
float rpm, req, e, ep, i, ip, p,joe,d;
int ot, nt, ot2;
void setup() {
  ep = 0;
  ip = 0;
  lcd.init();
  lcd.backlight();
  joe = 0;
  rpm = 0;
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(photo_pin),sw,FALLING);
}
void loop() {
  // put your main code here, to run repeatedly:
  lcd.print("RPM: "+String(rpm,3));
  req = map(analogRead(pot_pin),0,1023,0,255);
  Serial.println(digitalRead(swtch_pin));
  if (digitalRead(swtch_pin)==LOW) {
	lcd.setCursor(0,0);
	lcd.print("PID OFF");
	analogWrite(motor_pin, req);
  }
  else {
	if (millis()-ot2>=iteration_time) {
  	joe = pid(req,rpm);
  	ot2 = millis();
	}
	analogWrite(motor_pin, joe);
  }
}
void sw() {
  nt=millis();
  rpm = (nt-ot)*60000;
  ot=nt;
}
float pid(float sp, float pv) {
  if (margin(sp,pv,2)) return 0;
  e = pv-sp;
  p = e * scl;
  i = ip + e*iteration_time;
  d = (e-ep)/iteration_time;
  float out = p + i + d;
  ep = e;
  ip = i;
}
boolean margin(float a, float b, float m) {
  if (abs(a-b)<=m) return true;
  return false;
}

