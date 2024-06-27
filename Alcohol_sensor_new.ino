#include <SoftwareSerial.h>
SoftwareSerial sim(8, 9);
#include <TinyGPS++.h>
#include <LiquidCrystal.h>
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
float lattitude,longitude;
SoftwareSerial gpsSerial(0,1);
TinyGPSPlus gps;
int value;
#define motor 10
#define buzzer 13
#define led 7
String number = "+918059226129"; 
int a;
void setup()
{
pinMode(motor,OUTPUT);
pinMode(buzzer,OUTPUT);
pinMode(led,OUTPUT);
a=700;
Serial.begin(9600);
lcd.begin(16, 2);
lcd.setCursor(0,0);
lcd.print(" WELCOME ");
lcd.setCursor(0,1);
lcd.print(" ALCOHOL SENCEING ALERT");
sim.begin(9600);
gpsSerial.begin(9600); 
delay(6000);
lcd.clear();
}
void loop()
{
value=analogRead(A0); 
lcd.setCursor(0,0); 
lcd.print("value of alcohol"); 
lcd.setCursor(0,1);
lcd.print(value); 
delay(100); 
digitalWrite(motor, HIGH);
digitalWrite(buzzer, LOW);
digitalWrite(led, LOW);
if (value > a)
{ SendMessage();
}
}
void SendMessage()
{
digitalWrite(motor, LOW);
digitalWrite(buzzer, HIGH);
boolean newData = false;
for (unsigned long start = millis(); millis() - start < 2000;)
{
while (gpsSerial.available() > 0)
{
if (gps.encode(gpsSerial.read()))
{newData = true;}
}
}
if(newData)
{
Serial.print("Latitude= ");
Serial.print(gps.location.lat(), 6);
Serial.print(" Longitude= ");
Serial.println(gps.location.lng(), 6);
newData = false;
delay(300);

sim.println("AT+CMGF=1");
delay(200);
sim.println("AT+CMGS=\"" + number + "\"\r");
delay(200);
sim.print("http://maps.google.com/maps?q=loc:");
sim.print(gps.location.lat(), 6);
sim.print(",");
sim.print(gps.location.lng(), 6);
delay(100);
sim.println((char)26); 
delay(200);
Serial.println("GPS Location SMS Sent Successfully.");
lcd.clear();
lcd.setCursor(0,0);
lcd.print("Sending ... ");
lcd.setCursor(0,1);
lcd.print("Your Location");
delay(5000);
lcd.clear();
delay(200);
digitalWrite(buzzer, LOW);

while(1)
{ digitalWrite(led, HIGH);
delay(500);
digitalWrite(led, LOW);
digitalWrite(buzzer, HIGH);
delay(500);
digitalWrite(buzzer, LOW);
lcd.setCursor(0,0);
lcd.print(" High Alcohal ");
lcd.setCursor(0,1);
lcd.print(" Engine Locked ");}
}
}