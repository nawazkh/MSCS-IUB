#include "application.h"
//defining the ports.
int pirSensor = A0;
int led = D0;
int room = D6;

//defining the variables
int onValue;
int offValue;
int threshold;
int analogValue;
bool roomOccupied = false;
String data = String(10);
/*-----------------*/

void setup()
{
  pinMode(pirSensor, INPUT);
  pinMode(led, OUTPUT);
  pinMode(room, OUTPUT);

  Particle.variable("analogValue", &analogValue, INT);
  Particle.variable("threshold", &threshold, INT);
  Particle.variable("offValue", &offValue, INT);
  Particle.variable("onValue", &onValue, INT);

  //initial pull down
  digitalWrite(led,LOW);
  digitalWrite(room,LOW);

  //First, the LED will be on for 5 seconds alerting that you should not make any movements once it turns off
  digitalWrite(led,HIGH);
  delay(1250);
  digitalWrite(led,LOW);
  delay(1250);
  digitalWrite(led,HIGH);
  delay(1250);
  digitalWrite(led,LOW);
  delay(1250);
  //Capture the Off analog values of No Movement room.
  // Now we'll take some readings...
  analogValue = analogRead(pirSensor);
  int off_1 = analogRead(pirSensor);
  delay(2000);
  int off_2 = analogRead(pirSensor);
  delay(2000);
  //First, the LED will be on for 10 seconds alerting that you should MAKE movements once it turns off
  digitalWrite(led,HIGH);
  delay(1250);
  digitalWrite(led,LOW);
  delay(1250);
  digitalWrite(led,HIGH);
  delay(1250);
  digitalWrite(led,LOW);
  delay(1250);
  digitalWrite(led,HIGH);
  delay(1250);
  digitalWrite(led,LOW);
  delay(1250);
  digitalWrite(led,HIGH);
  delay(1250);
  digitalWrite(led,LOW);
  delay(1250);
  //just wait
  delay(3000);
  digitalWrite(led,HIGH);
  delay(250);
  digitalWrite(led,LOW);
  delay(250);
  //start
  // Now we'll take some readings...
  analogValue = analogRead(pirSensor);
  int on_1 = analogRead(pirSensor);
  delay(2000);
  int on_2 = analogRead(pirSensor);
  delay(2000);
  //readings taken
  digitalWrite(led,HIGH);
  delay(1250);
  digitalWrite(led,LOW);
  delay(1250);
  digitalWrite(led,HIGH);
  delay(1250);
  digitalWrite(led,LOW);
  delay(1250);
  // Now we average the "on" and "off" values to get an idea of what the resistance will be when the LED is on and off
  onValue = (on_1+on_2)/2;
  offValue = (off_1+off_2)/2;

  // Let's also calculate the value between ledOn and ledOff, above which we will assume the led is on and below which we assume the led is off.
  threshold = (onValue+offValue)/2;
}


void loop(){
  analogValue = analogRead(pirSensor);
  if( analogValue > threshold){
    if(roomOccupied==false){
      //room has activity
      roomOccupied = true;
      digitalWrite(led,HIGH);
      delay(250);
      digitalWrite(led,LOW);
      delay(250);
      data = "Occupied";
      Particle.publish("RoomOccupy",data ,60, PRIVATE);
      digitalWrite(room,HIGH);

    }
    else{
      //do nothing
      digitalWrite(room,HIGH);

    }
  }
  else{
    if(roomOccupied==true){
      //room has no activity
      roomOccupied = false;
      digitalWrite(room,LOW);
      digitalWrite(led,HIGH);
      delay(250);
      digitalWrite(led,LOW);
      delay(250);
      data = "Free";
      Particle.publish("RoomOccupy", data,60, PRIVATE);

    }
    else{
      digitalWrite(room,LOW);

    }
  }
}


/*-----------------*/
/*-----------------*/
