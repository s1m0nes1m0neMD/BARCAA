int enableB = 7;
int enableA = 3;
int pinA1 = 4;
int pinA2 = 5;
int pinB1 = 6;
int pinB2 = 8;
boolean run;
#define echoPin 2 // Echo Pin
#define trigPin A0 // Trigger Pin
#define stopfar 13 // Stop Farlar
#define gerivitesfar 12 // gerivites Farlar
#define onfarlar 11 // ön farlar
int duration, distance; // Duration used to calculate distance
#include <SoftwareSerial.h>
SoftwareSerial BT(9, 10); //TX, RX respetively
String readdata;
void setup() {
   BT.begin(9600);
 Serial.begin(9600);
  pinMode(enableA, OUTPUT);
  pinMode(pinA1, OUTPUT);
  pinMode(pinA2, OUTPUT);
  pinMode(enableB, OUTPUT);
  pinMode(pinB1, OUTPUT);
  pinMode(pinB2, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(stopfar, OUTPUT); // frene basıldığı zaman yanacak Stop Farlar
  pinMode(gerivitesfar, OUTPUT); // geriye basıldığı zaman yanacak gerivites Farlar
  pinMode(onfarlar, OUTPUT); // ileri gaza basınca ön farlar yansın
  run = true;
}

void loop() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);

  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);

  //Calculate the distance (in cm) based on the speed of sound.
  distance = duration / 58.2;
  if ( distance < 30) {

    BT.print(distance);
    backward(20);
    coast(10);
//    BT.print("çok yakın: ");
//    BT.print(distance);
//    BT.println("cm");
//    readdata = "stop";
  }
  if (run)
  {
    delay(2000);
  enableMotors();
    forward(100);
    coast(500);
    brake(500);
    backward(100);
    coast(500);
    brake(500);
    //   turnLeft(1500);
    //    brake(500);
    //    turnRight(1500);
    //    brake(500);
    //  disableMotors();
    run = false;
  }
  while (BT.available()) { //Check if there is an available byte to read
    delay(10); //Delay added to make thing stable
    char c = BT.read(); //Conduct a serial read
    readdata += c; //build the string- "forward", "reverse", "left" and "right"
  }
  if (readdata.length() > 0) {
     enableMotors();
    Serial.println(readdata);

    if (readdata == "f")
    {
      digitalWrite(onfarlar, HIGH); digitalWrite(stopfar, LOW);
     
    }
    else  if (readdata == "g")
    {
      digitalWrite(onfarlar, LOW); digitalWrite(stopfar, LOW);
    
    }
    else  if (readdata == "a")
    {
      forward(20);coast(10);
   
         digitalWrite(stopfar, LOW); digitalWrite(gerivitesfar, LOW);
      
    }

    else  if (readdata == "b")
    {
      backward(20);coast(10);
     
      digitalWrite(stopfar, LOW); digitalWrite(gerivitesfar, HIGH);
     
    }

    else  if (readdata == "c")
    {
       turnRight(80);
     
      digitalWrite(stopfar, LOW); digitalWrite(gerivitesfar, LOW);
      
    }

    else  if ( readdata == "d")
    {
       turnLeft(80);
     
    digitalWrite(stopfar, LOW); digitalWrite(gerivitesfar, LOW);
    
    }

    else  if (readdata == "e")
    {
    brake(20);coast(10);
      digitalWrite(stopfar, HIGH); digitalWrite(gerivitesfar, LOW);
    
    }
    else
      readdata = "";
  }

}
//Define Low Level H-Bridge Commands
//enable motors
void motorAOn()
{
  digitalWrite(enableA, HIGH);
}
void motorBOn()
{
  digitalWrite(enableB, HIGH);
}
//disable motors
void motorAOff()
{
  digitalWrite(enableB, LOW);
}
void motorBOff()
{
  digitalWrite(enableA, LOW);
}
//motor A controls
void motorAForward()
{
  digitalWrite(pinA1, HIGH);
  digitalWrite(pinA2, LOW);
}
void motorABackward()
{
  digitalWrite(pinA1, LOW);
  digitalWrite(pinA2, HIGH);
}
//motor B contorls
void motorBForward()
{
  digitalWrite(pinB1, HIGH);
  digitalWrite(pinB2, LOW);
}
void motorBBackward()
{
  digitalWrite(pinB1, LOW);
  digitalWrite(pinB2, HIGH);
}
//coasting and braking

void motorACoast()
{
  digitalWrite(pinA1, LOW);
  digitalWrite(pinA2, LOW);
}
void motorABrake()
{
  digitalWrite(pinA1, HIGH);
  digitalWrite(pinA2, HIGH);
}
void motorBCoast()
{
  digitalWrite(pinB1, LOW);
  digitalWrite(pinB2, LOW);
}
void motorBBrake()
{
  digitalWrite(pinB1, HIGH);
  digitalWrite(pinB2, HIGH);
}
//Define High Level Commands
void enableMotors()
{
  motorAOn();
  motorBOn();
}
void disableMotors()
{
  motorAOff();
  motorBOff();
}
void forward(int time)
{
  motorAForward();
  motorBForward();
  delay(time);
}
void backward(int time)
{
  motorABackward();
  motorBBackward();
  delay(time);
}
void turnLeft(int time)
{
  motorABackward();
  motorBForward();
  delay(time);
}
void turnRight(int time)
{
  motorAForward();
  motorBBackward();
  delay(time);
}
void coast(int time)
{
  motorACoast();
  motorBCoast();
  delay(time);
}
void brake(int time)
{
  motorABrake();
  motorBBrake();
  delay(time);
}
