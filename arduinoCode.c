#include <SoftwareSerial.h>
/*#include <DateTime.h>
#include <DateTimeStrings.h>*/
#include <dht.h>
dht DHT;
#define DHT11_PIN 11

String readdata;
const int trigPin = 3, echoPin = 12, ledPin = 28;
int rh=4, rl=5, lh=7, ll=6,
snsr1ip=0,
snsr1op=13,
snsr2ip=1,
snsr2op=8;
long duration;
int distance;
int safetyDistance;
int motortemp;
int x1=0, x2=0,  v1, v2, trig=0;
float t1, t2;
float vout = 0.0;
float vin = 0.0;
float R1 = 80000.0;  
float R2 = 20000.0; 
int value = 0;

void setup() 
{
  Serial.begin(9600);
  pinMode(rh, OUTPUT);
  pinMode(rl, OUTPUT);
  pinMode(lh, OUTPUT);
  pinMode(ll, OUTPUT);
  pinMode(snsr1op, OUTPUT);
  pinMode(snsr2op, OUTPUT);
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  pinMode(ledPin, OUTPUT);
  pinMode(9,INPUT);
  digitalWrite(rh,LOW);
  digitalWrite(lh,LOW);
  digitalWrite(rl,LOW);
  digitalWrite(ll,LOW);
  v2= map(analogRead(4),45,63,20,35);
}

void loop() 
{

   value = analogRead(3);
   vout = (value * 4.5) / 1024.0; // see text 
   vin = vout / (R2/(R1+R2)); 
   Serial.print("INPUT V= ");
   Serial.println(vin,2);
   delay(500);

   motortemp = map(analogRead(4),45,63,20,35);
   v1=(motortemp+v2)/2;
   v2=motortemp;
   Serial.print("motor temp = ");
   Serial.println(v1);
   delay(500);
// car sensor  
   if(digitalRead(9)==0)
   {digitalWrite(rh, HIGH); digitalWrite (lh, HIGH); digitalWrite(rl,LOW); digitalWrite(ll,LOW); delay (500);} 
  // Serial.println("Start Car");
//---------------------------------------------------------------------------//

// light sensor
   digitalWrite (snsr1op, LOW);
   while (analogRead(snsr1ip)<30) digitalWrite (snsr1op, HIGH);
   while (analogRead(snsr1ip)>300) {digitalWrite(rh, HIGH); digitalWrite (lh, HIGH); digitalWrite(rl,LOW); digitalWrite(ll,LOW); x2=1;}
   if (analogRead(snsr1ip)>300 && x2==1) {digitalWrite(rh, HIGH); digitalWrite (lh, HIGH); x2 =0;}
//   Serial.println(analogRead(snsr1ip));
//---------------------------------------------------------------------------//

// flame sensor
   digitalWrite (snsr2op, LOW);
   while (analogRead(snsr2ip)<700) digitalWrite (snsr2op, HIGH);
//----------------------------------------------------------------------------//

// temp & humidity
   int chk = DHT.read11(DHT11_PIN);
   Serial.print("Temperature = ");
//   Serial.print("|");
   Serial.println(DHT.temperature);
   Serial.print("Humidity = ");
   //Serial.print("|");
   Serial.print(DHT.humidity);
Serial.println("%");
//----------------------------------------------------------------------------//

// ultrasonic
digitalWrite(trigPin, LOW);
delayMicroseconds(2);

// Sets the trigPin on HIGH state for 10 micro seconds
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);

// Reads the echoPin, returns the sound wave travel time in microseconds
duration = pulseIn(echoPin, HIGH);

// Calculating the distance
distance= duration*0.034/2;
//Serial.println(distance);
safetyDistance = distance;
if (distance <= 30){

     digitalWrite (rh, LOW); digitalWrite (lh, LOW); digitalWrite (rl, LOW); digitalWrite (ll, LOW); delay (10);
}
/*else{
 // digitalWrite(buzzer, LOW);
   digitalWrite(ledPin, LOW);
}*/
// Prints the distance on the Serial Monitor
   Serial.print("Distance: ");
   Serial.println(distance);
   delay(500);
//----------------------------------------------------------------------------//

/*if(x2==0 && trig == 1 && distance!=x1)
{
  x2=distance;
  Serial.print("speed = ");
  Serial.print(abs(x2-x1) / 500);
  x1= 0;
  trig = 0;
}*/

//main*/
if (Serial.available())
{
  readdata = Serial.readString();
  Serial.println(readdata);
 
  /*//if(readdata == "s")
 // {
  if(x1==0)
  {x1= distance; trig=1;}
  //}*/
  
   if(readdata == "forward")
  {
   /* for ( int i =64 ; i < 1023; i=i*2)
    { analogWrite(rh, i); analogWrite (lh, i); digitalWrite(rl,LOW); digitalWrite(ll,LOW); delay(350); }*/
     digitalWrite(rh, HIGH); digitalWrite (lh, HIGH); digitalWrite(rl,LOW); digitalWrite(ll,LOW); //delay(50);
  }

  else if(readdata == "reverse")
  {
    digitalWrite(rh, LOW); digitalWrite(lh, LOW); digitalWrite(rl, HIGH); digitalWrite(ll,HIGH); //delay(50);
  }

  else if (readdata == "left")
  {
    digitalWrite (rh,HIGH); digitalWrite (rl,LOW); digitalWrite (lh,LOW); digitalWrite (ll,LOW); //delay (50);
   
  }

 else if ( readdata == "right")
 {
   digitalWrite (rh, LOW); digitalWrite (lh, HIGH); digitalWrite (rl, LOW); digitalWrite (ll, LOW); //delay (50);
 }

 else if (readdata == "stop")
 {
  digitalWrite (rh, LOW); digitalWrite (lh, LOW); digitalWrite (rl, LOW); digitalWrite (ll, LOW); //delay (10);
 }

  
readdata="";}

}
