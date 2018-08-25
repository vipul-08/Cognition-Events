
//MATRIX CODE

#include <WiFiUdp.h>
#include <ESP8266WiFi.h>


#define p1 D1 
#define p2 D2 
#define p3 D3 
#define p4 D4 
#define p5 D0

const char* ssid = "vipul"; //Enter your wifi network SSID
const char* password ="12345678"; //Enter your wifi network password


const int SERVER_PORT = 1111;
const int BAUD_RATE = 115200;

byte incomingByte = 0;

bool playerA = false;
bool playerB = false;
bool playerC = false;
bool playerD = false;
bool resetAll = false;

bool set = false;

const int A_PRESSED = 49;
const int B_PRESSED = 50;
const int C_PRESSED = 51;
const int D_PRESSED = 52;
const int RESET_PRESSED = 53;

byte packetBuffer[512];

WiFiUDP Udp;
IPAddress ip;

void initOutputs() {
  pinMode(p1,OUTPUT);
  pinMode(p2,OUTPUT);
  pinMode(p3,OUTPUT);
  pinMode(p4,OUTPUT);
  pinMode(p5,OUTPUT);
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

void detectKeyPresses() {
    if (incomingByte == A_PRESSED) {
      playerA = true;
      playerB = false;
      playerC = false;
      playerD = false;
      resetAll = false;
    }
    else if (incomingByte == B_PRESSED) {
      playerA = false;
      playerB = true;
      playerC = false;
      playerD = false;
      resetAll = false;
    }
    else if (incomingByte == C_PRESSED) {
      playerA = false;
      playerB = false;
      playerC = true;
      playerD = false;
      resetAll = false;
    }
    else if (incomingByte == D_PRESSED) {
      playerA = false;
      playerB = false;
      playerC = false;
      playerD = true;
      resetAll = false;
    }
    else if(incomingByte == RESET_PRESSED) {
      playerA = false;
      playerB = false;
      playerC = false;
      playerD = false;
      resetAll = true;
    }
}

void handlePinOutputs() {
  if (playerA == true && set == false) 
  { 
    digitalWrite(p1,HIGH);
    digitalWrite(p5,HIGH);
    Serial.println("Player1");
    set = true;
  }
  else if (playerB == true && set == false) 
  {
    digitalWrite(p2,HIGH);
    digitalWrite(p5,HIGH);
    Serial.println("Player2");
    set = true;
  }
  else if(playerC == true && set == false)
  {
    digitalWrite(p3,HIGH);
    digitalWrite(p5,HIGH);
    Serial.println("Player3");
    set = true;
  }
  else if(playerD == true && set == false)
  {
    digitalWrite(p4,HIGH);
    digitalWrite(p5,HIGH);
    Serial.println("Player4");
    set = true;
  }
  else if(resetAll == true )
  {
    digitalWrite(p1,LOW);
    digitalWrite(p2,LOW);
    digitalWrite(p3,LOW);
    digitalWrite(p4,LOW);
    digitalWrite(p5,LOW);
    Serial.println("Reset");
    set = false;
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