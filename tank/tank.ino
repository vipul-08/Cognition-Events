#include <WiFiUdp.h>
#include <ESP8266WiFi.h>
#include<Servo.h>

Servo myservo;

#define m1 D1 
#define m2 D2 
#define m3 D3 
#define m4 D4 
#define b1 D6

const char* ssid ="thisteambytes"; //Enter your wifi network SSID
const char* password ="12345678"; //Enter your wifi network password


const int SERVER_PORT = 1111;
const int BAUD_RATE = 115200;

byte incomingByte = 0;

bool forwardsPressed = false;
bool backwardsPressed = false;
bool rightPressed = false;
bool leftPressed = false;
bool buttonPressed = false;

const int FORWARDS_PRESSED = 1;
const int FORWARDS_RELEASED = 2;
const int BACKWARDS_PRESSED = 3;
const int BACKWARDS_RELEASED = 4;
const int RIGHT_PRESSED = 5;
const int RIGHT_RELEASED = 6;
const int LEFT_PRESSED = 7;
const int LEFT_RELEASED = 8;
const int BUTTON_PRESSED = 9;
const char BUTTON_RELEASED = 0;


byte packetBuffer[512];

WiFiUDP Udp;
IPAddress ip;

void initOutputs() {
  pinMode(m1,OUTPUT);
  pinMode(m2,OUTPUT);
  pinMode(m3,OUTPUT);
  pinMode(m4,OUTPUT);
  pinMode(b1,OUTPUT);
  digitalWrite(b1,LOW);
 
}

void connectWifi() {
  Serial.println();
  Serial.println();
  Serial.print("Connecting to WIFI network");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
  Udp.begin(SERVER_PORT);
  ip = WiFi.localIP();
  Serial.println(ip);
}

void moveForwards() {
  Serial.println("Forward");
  analogWrite(m1,1024);
  analogWrite(m2,0);
  analogWrite(m3,1024);
  analogWrite(m4,0);
}

void moveBackwards() {
  Serial.println("Backwards");
  analogWrite(m1,0);
  analogWrite(m2,1024);
  analogWrite(m3,0);
  analogWrite(m4,1024);
}

void turnRight() {
  Serial.println("Hard Right");
  analogWrite(m1,1024);
  analogWrite(m2,0);
  analogWrite(m3,0);
  analogWrite(m4,1024);
}

void turnLeft() {
  Serial.println("Hard Left");
  analogWrite(m1,0);
  analogWrite(m2,1024);
  analogWrite(m3,1024);
  analogWrite(m4,0);
}


void softLeftF()
{
  Serial.println("Soft Left F");
  analogWrite(m1,0);
  analogWrite(m2,0);
  analogWrite(m3,1024);
  analogWrite(m4,0);
}

void softLeftB()
{
  Serial.println("Soft left B");
  analogWrite(m1,0);
  analogWrite(m2,0);
  analogWrite(m3,0);
  analogWrite(m4,1024);
}

void softRightF()
{
  Serial.println("Soft Right F");
  analogWrite(m1,1024);
  analogWrite(m2,0);
  analogWrite(m3,0);
  analogWrite(m4,0);
}
void softRightB()
{
  Serial.println("Soft Right B");
  analogWrite(m1,0);
  analogWrite(m2,1024);
  analogWrite(m3,0);
  analogWrite(m4,0);
}
void gunControl() {
  Serial.println("Button On");
  digitalWrite(b1,HIGH);
}

void gunReset(){
  Serial.println("Button Off");
  digitalWrite(b1,LOW);
}

void resetStewwwwwwwwwwwering() {
  Serial.println("reset s");
}

void resetEngine() {
  Serial.println("reset e");
  analogWrite(m1,0);
  analogWrite(m2,0);
  analogWrite(m3,0);
  analogWrite(m4,0);
}

void detectKeyPresses() {
  if (incomingByte == FORWARDS_PRESSED) {
      forwardsPressed = true;
    }
    else if (incomingByte == BACKWARDS_PRESSED) {
      backwardsPressed = true;
    }

    if (incomingByte == FORWARDS_RELEASED) {
      forwardsPressed = false;
    }
    else if (incomingByte == BACKWARDS_RELEASED) {
      backwardsPressed = false;
    }

    if (incomingByte == RIGHT_PRESSED) {
      rightPressed = true;
    }
    else if (incomingByte == LEFT_PRESSED) {
      leftPressed = true;
    }
    if (incomingByte == RIGHT_RELEASED) {
      rightPressed = false;
    }
    else if (incomingByte == LEFT_RELEASED) {
      leftPressed = false;
      
    }
    if (incomingByte == BUTTON_PRESSED) {
      buttonPressed = true;
    }
    else if (incomingByte == BUTTON_RELEASED) {
      buttonPressed = false;
    }
}

void handlePinOutputs() {
  if (forwardsPressed == true) 
  {
     if(rightPressed == true)
      {
        softRightF();
      }
     else if(leftPressed == true)
     {
       softLeftF();
     }
    else 
      moveForwards();
  }
  else if (backwardsPressed == true) 
  {
    if(rightPressed == true)
    {
      softRightB();
    }
    else if(leftPressed == true)
    {
      softLeftB();
    }
    else
    moveBackwards();
  }
  else if(rightPressed == true)
  {
    turnRight();
  }
  else if(leftPressed == true)
  {
    turnLeft();
  }
  else {
    resetEngine();
  }


  if(buttonPressed == true) 
  {
    gunControl();
  }
  else if(buttonPressed == false) 
  {
    gunReset();
  }
}

void setup() {
  Serial.begin(BAUD_RATE);
  delay(10);
  initOutputs();
  connectWifi();
}

void loop() {
  int noBytes = Udp.parsePacket();
  String received_command = "";
  if ( noBytes ) 
  {
    Serial.print(millis() / 1000);
    Serial.print(":Packet of ");
    Serial.print(noBytes);
    Serial.print(" received from ");
    Serial.print(Udp.remoteIP());
    Serial.print(":");
    Serial.println(Udp.remotePort());

    Udp.read(packetBuffer,noBytes);
    Serial.println();

    Serial.println(packetBuffer[0]);
    incomingByte = packetBuffer[0];
    Serial.println();

    detectKeyPresses();
    handlePinOutputs();
  }
  
}

