//ROBO SOCCER CODE
#include <WiFiUdp.h>
#include <ESP8266WiFi.h>
#include<Servo.h>

Servo myservo;

#define m1 D0
#define m2 D1 
#define m3 D2 
#define m4 D3 
#define b1 D4

const char* ssid = "Markiv"; //Enter your wifi network SSID
const char* password ="87654321"; //Enter your wifi network password


const int SERVER_PORT = 1111;
const int BAUD_RATE = 115200;

byte incomingByte = 0;

bool forwardsPressed = false;
bool backwardsPressed = false;
bool rightPressed = false;
bool leftPressed = false;

bool forwardLeftPressed = false;
bool forwardRightPressed = false;

bool backwardLeftPressed = false;
bool backwardRightPressed = false;

bool buttonPressed = false;

const int RIGHT_PRESSED = 49;
const int FORWARD_RIGHT_PRESSED = 50;
const int FORWARDS_PRESSED = 51;
const int FORWARD_LEFT_PRESSED = 52;
const int LEFT_PRESSED = 53;
const int BACKWARD_LEFT_PRESSED = 54;
const int BACKWARDS_PRESSED = 55;
const int BACKWARD_RIGHT_PRESSED = 56;

const int U_RESET = 57;

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

void resetSteering() {
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
      leftPressed = false;
      rightPressed = false;
      forwardsPressed = true;
      backwardsPressed = false;
      forwardLeftPressed = false;
      forwardRightPressed = false;
      backwardLeftPressed = false;
      backwardRightPressed = false;
    }
    else if (incomingByte == BACKWARDS_PRESSED) {
      leftPressed = false;
      rightPressed = false;
      forwardsPressed = false;
      backwardsPressed = true;
      forwardLeftPressed = false;
      forwardRightPressed = false;
      backwardLeftPressed = false;
      backwardRightPressed = false;
    }
    else if (incomingByte == RIGHT_PRESSED) {
      leftPressed = false;
      rightPressed = true;
      forwardsPressed = false;
      backwardsPressed = false;
      forwardLeftPressed = false;
      forwardRightPressed = false;
      backwardLeftPressed = false;
      backwardRightPressed = false;
    }
    else if (incomingByte == LEFT_PRESSED) {
      leftPressed = true;
      rightPressed = false;
      forwardsPressed = false;
      backwardsPressed = false;
      forwardLeftPressed = false;
      forwardRightPressed = false;
      backwardLeftPressed = false;
      backwardRightPressed = false;
    }
    else if(incomingByte == FORWARD_LEFT_PRESSED) {
      leftPressed = false;
      rightPressed = false;
      forwardsPressed = false;
      backwardsPressed = false;
      forwardLeftPressed = true;
      forwardRightPressed = false;
      backwardLeftPressed = false;
      backwardRightPressed = false;
    }
    else if(incomingByte == BACKWARD_LEFT_PRESSED) {
      leftPressed = false;
      rightPressed = false;
      forwardsPressed = false;
      backwardsPressed = false;
      forwardLeftPressed = false;
      forwardRightPressed = false;
      backwardLeftPressed = true;
      backwardRightPressed = false;
    }
    else if(incomingByte == FORWARD_RIGHT_PRESSED) {
      leftPressed = false;
      rightPressed = false;
      forwardsPressed = false;
      backwardsPressed = false;
      forwardLeftPressed = false;
      forwardRightPressed = true;
      backwardLeftPressed = false;
      backwardRightPressed = false;
    }
    else if(incomingByte == BACKWARD_RIGHT_PRESSED) {
      leftPressed = false;
      rightPressed = false;
      forwardsPressed = false;
      backwardsPressed = false;
      forwardLeftPressed = false;
      forwardRightPressed = false;
      backwardLeftPressed = false;
      backwardRightPressed = true;
    }
    else if(incomingByte == U_RESET) {
      leftPressed = false;
      rightPressed = false;
      forwardsPressed = false;
      backwardsPressed = false;
      forwardLeftPressed = false;
      forwardRightPressed = false;
      backwardLeftPressed = false;
      backwardRightPressed = false;
    }
    
}

void handlePinOutputs() {
  if (forwardsPressed == true) 
  { 
      moveForwards();
  }
  else if (backwardsPressed == true) 
  {
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
  else if(forwardLeftPressed == true)
  {
    softLeftF();
  }
  else if(backwardLeftPressed == true)
  {
    softLeftB();
  }
  else if(forwardRightPressed == true)
  {
    softRightF();
  }
  else if(backwardRightPressed == true)
  {
    softRightB();
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

    if(noBytes >1 ) {
      incomingByte = 0;
      Serial.println("Greater than 1..");
      //Serial.println(noBytes);
      for(int i=0;i<noBytes ; i++) {
        int flag;
        if(i == 0)
          flag = 10;
        else 
          flag = 1;
        incomingByte = incomingByte + ((packetBuffer[i] - 48)*flag);
        
      }
      incomingByte = incomingByte + 48;
      Serial.println(incomingByte);
      Serial.println();
      
    } else {
      
      Serial.println(packetBuffer[0]);
      incomingByte = packetBuffer[0];
      Serial.println();
    
    }
    detectKeyPresses();
    handlePinOutputs();
  }
  
}