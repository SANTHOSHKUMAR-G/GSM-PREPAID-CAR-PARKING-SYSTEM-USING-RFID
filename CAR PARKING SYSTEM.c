#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include <SoftwareSerial.h>
SoftwareSerial mySerial(11,10);
const int irSensorPin1 =2 ;
const int irSensorPin2 =3 ;
const int irSensorPin3 =8 ;
const int emptyLedPin1 = 4;
const int fullLedPin1 = 5;
const int emptyLedPin2 = 6;
const int fullLedPin2 =7;
const int emptyLedPin3 = 12;
const int fullLedPin3 =13;
int pos = 0;
int val;
int count = 0;
char input[12];
int LED = 13;
LiquidCrystal_I2C lcd(0x3F, 16, 2);
const float chargeAmount = 50;
int total = 1000;
Servo myservo;
String recipientNumber = "+918825793101";
int servoPin = 9;
void setup()
{
Serial.begin(9600);
mySerial.begin(9600);
lcd.begin(16, 2);
myservo.attach(servoPin);
lcd.init();
lcd.backlight();
lcd.begin(16,2);
lcd.print("KCE Tollgate");
lcd.setCursor(0, 1);
lcd.print("System");
pinMode(irSensorPin1, INPUT);
pinMode(irSensorPin2, INPUT);
pinMode(irSensorPin3, INPUT);
pinMode(emptyLedPin1, OUTPUT);
pinMode(fullLedPin1, OUTPUT);
pinMode(emptyLedPin2, OUTPUT);
pinMode(fullLedPin2, OUTPUT);
pinMode(emptyLedPin3, OUTPUT);
pinMode(fullLedPin3, OUTPUT);
lcd.clear();
mySerial.println("AT");
delay(1000);
if (mySerial.find("OK"))
{
Serial.println("GSM module is ready.");
}
else
{
Serial.println("Error: GSM module is not responding.");
 while (1); // Halt the program
}
delay(1000);
}
void loop()
{
ir();
lcd.clear();
lcd.setCursor(1, 0);
lcd.print("Place Card");
delay(1000);
if (Serial.available()) {
int count = 0;
while (Serial.available() && count < 12)
{
input[count] = Serial.read();
count++;
delay(5);
}
if (input[0] == '4' && input[1] == 'D' && input[2] == '0' && input[3] == '0'
&& input[4] == '8' && input[5] == 'B' && input[6] == 'A' && input[7] == 'D'
&& input[8] == '6' && input[9] == '0' && input[10] == '0' && input[11] =='B')
{
lcd.clear();
lcd.print("Card Accepted");
delay(1000);
lcd.clear();
if(total>chargeAmount)
{
lcd.print("Total:");
lcd.print(total);
delay(2000);
lcd.clear();
total -= chargeAmount;
lcd.clear();
lcd.print("Charge Amount:");
lcd.print(chargeAmount);
delay(2000);
lcd.setCursor(0, 1);
lcd.print("Balance: ");
lcd.print(total);
delay(200);
delay(5000);
}
// Open the gate
myservo.write(180);
mySerial.print("AT+CMGF=1\r"); // Set SMS mode to text
delay(1000);
mySerial.print("AT+CMGS=\"" + recipientNumber + "\"\r");
delay(1000); // Keep the gate open for 1 second
 String message = "Welcome to KCE parking \ncharged
amount:"+String(chargeAmount)+"\nBalance Amount:"+String(total);
mySerial.print(message);
delay(1000);
mySerial.write(26);
// Close the gate
myservo.write(60);
}
else
{
lcd.clear();
lcd.print("Total:");
lcd.print(total);
delay(2000);
lcd.print("invalid Amount");
sentSMS2();
}
}
else
{
lcd.clear();
lcd.print("Invalid Card");
delay(1000);
}
lcd.clear();
lcd.print("THANK YOU FOR VISIT");
delay(3000);
}
}
void ir()
{
lcd.clear();
int irValue1 = digitalRead(irSensorPin1);
int irValue2 = digitalRead(irSensorPin2);
int irValue3 = digitalRead(irSensorPin3);
if (irValue1 == LOW)
{
digitalWrite(emptyLedPin1, LOW);
digitalWrite(fullLedPin1, HIGH);
lcd.setCursor(1, 0);
lcd.print("SLOT 1 IS FULL");
delay(1000);
}
else
{
digitalWrite(emptyLedPin1, HIGH);
digitalWrite(fullLedPin1, LOW);
lcd.setCursor(1, 0);
lcd.print("SLOT 1 IS EMPTY");
delay(1000);
}
if (irValue2 == LOW)
{
digitalWrite(emptyLedPin2, LOW);
digitalWrite(fullLedPin2, HIGH);
lcd.setCursor(1, 1);
lcd.print("SLOT 2 IS FULL");
delay(1000);
}
else
{
digitalWrite(emptyLedPin2, HIGH);
digitalWrite(fullLedPin2, LOW);
lcd.setCursor(1, 1);
lcd.print("SLOT 2 IS EMPTY");
delay(1000);
}
lcd.clear();
if (irValue3 == LOW)
{
digitalWrite(emptyLedPin3, LOW);
digitalWrite(fullLedPin3, HIGH);
lcd.setCursor(1, 0);
lcd.print("SLOT 3 IS FULL");
delay(1000);
}
else
{
digitalWrite(emptyLedPin3, HIGH);
digitalWrite(fullLedPin3, LOW);
lcd.setCursor(1, 0);
lcd.print("SLOT 3 IS EMPTY");
delay(1000);
}
}
void sentSMS1()
{
delay(1000); // Keep the gate open for 1 second
String message = "Welcome to KCE parking \ncharged
amount:"+String(chargeAmount)+"\nBalance Amount:"+String(total);
mySerial.print(message);
delay(1000);
if (mySerial.find("OK"))
{
Serial.println("Message sent successfully!");
}
else
{
Serial.println("Error: Failed to send message.");
}
}
void sentSMS2()
{
delay(1000); // Keep the gate open for 1 second
String message = "Welcome to KCE parking\nBalance
Amount:"+String(total)+"Invalid Amount\nplease recharge your card";
mySerial.print(message);
delay(1000);
}
