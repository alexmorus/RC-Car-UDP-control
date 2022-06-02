#include <Wire.h>
#include <MCP342x.h>
#include <WiFiUdp.h>
#include <ESP8266WiFi.h>

#define WIFI_SSID "NodeMCU_Server"
#define WIFI_PASS "12345678"

#define UDP_PORT 4210
IPAddress receivingIp(192,168,4,173);

uint8_t address = 0x68;
MCP342x adc = MCP342x(address);

WiFiUDP UDP;

//char packet[255];

//uint8_t valoriButon[] = [13, 126, 224, 391, 514, 655, 764, 865]; 18 254 452 792 1044 1330 1550 1758
// joystick dreapta sus-jos: jos maxim: 2050, sus maxim: 20100, canal 1
// joystick dreapta stanga-dreapta: dreapta maxim: 3600, stanga maxim: 20000, canal 2
// joystick stanga stanga-dreapta: stanga maxim: 19500, dreapta maxim: 3400, canal 3
// joystick stanga sus-jos: sus maxim: 20750, dreapta maxim: 2800, canal 4

uint8_t getButon();
uint8_t readButon();
uint8_t readLeftUpDown();
uint8_t readLeftSides();
uint8_t readRightUpDown();
uint8_t readRightSides();


void setup() {

  Serial.begin(9600);
  Wire.begin(D2, D1);

  MCP342x::generalCallReset();
  delay(5);

  Wire.requestFrom(address, (uint8_t)1);
  if (!Wire.available()) {
    Serial.print("No device found at address ");
    Serial.println(address, HEX);
    while (1)
      ;
  }

  WiFi.softAP(WIFI_SSID, WIFI_PASS);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);


//  WiFi.begin(WIFI_SSID, WIFI_PASS);
  
//  Serial.print("Conectare la ");
//  Serial.print(WIFI_SSID);
//  // Loop continuously while WiFi is not connected
//  while (WiFi.status() != WL_CONNECTED)
//  {
//    delay(100);
//    Serial.print(".");
//  }
//
//  Serial.println();
//  Serial.print("Conexiune reusita! Adresa IP: ");
//  Serial.println(WiFi.localIP()); 

  UDP.begin(UDP_PORT);
  Serial.print("Conectat la portul ");
  Serial.println(UDP_PORT);
}

void loop() {
  
  uint8_t buton = readButon();
  if (buton != 255) {
    Serial.print("A fost apasat butonul");
    Serial.println(buton);
  }
  
//  Serial.print("Joystick: ");
//  Serial.println(readRightUpDown());

  char packet[255];
  sprintf(packet, "%d-%d", readLeftUpDown(), readRightSides());

//  Serial.println(packet);
  UDP.beginPacket(receivingIp, UDP_PORT);
  UDP.write(packet);
  UDP.endPacket();

  delay(10);
}

uint8_t readLeftUpDown() { // 95-110 
    MCP342x::Config status;

    long value = 0;
    uint8_t joystick1 = adc.convertAndRead(MCP342x::channel4, MCP342x::oneShot,
                                           MCP342x::resolution12, MCP342x::gain1,
                                           1000000, value, status);
//    Serial.println(value);
    return map(value, 170, 1300, 0, 255);
}

uint8_t readLeftSides() { // 100-115
    MCP342x::Config status;

    long value = 0;
    uint8_t joystick1 = adc.convertAndRead(MCP342x::channel3, MCP342x::oneShot,
                                           MCP342x::resolution12, MCP342x::gain1,
                                           1000000, value, status);
//    Serial.println(value);

    return map(value, 210, 1240, 0, 255);
}

uint8_t readRightSides() { // 100-110
    MCP342x::Config status;

    long value = 0;
    uint8_t joystick1 = adc.convertAndRead(MCP342x::channel2, MCP342x::oneShot,
                                           MCP342x::resolution12, MCP342x::gain1,
                                           1000000, value, status);
//    Serial.println(value);

    return map(value, 210, 1260, 0, 255);
}

uint8_t readRightUpDown() { // 100-115
    MCP342x::Config status;

    long value = 0;
    uint8_t joystick1 = adc.convertAndRead(MCP342x::channel1, MCP342x::oneShot,
                                           MCP342x::resolution12, MCP342x::gain1,
                                           1000000, value, status);
//    Serial.println(value);

    return map(value, 130, 1250, 0, 255);
}


uint8_t getButon() {
    int buton = analogRead(A0);
    buton = map(buton, 0, 1024, 0, 2048);

//    Serial.println(buton);

    return (buton/100);  
}

uint8_t readButon() {
    int buton = getButon();
    if (buton == 0) {
      return 0;
    } else if (buton == 2) {
      return 1;
    } else if (buton == 4) {
      return 2;
    } else if (buton == 7 || buton == 8) {
      return 3;
    } else if (buton == 10) {
      return 4;
    } else if (buton == 13) {
      return 5;
    } else if (buton == 15) {
      return 6;
    } else if (buton == 17) {
      return 7;
    } else if (buton == 20) {
      return 255;
    }
}
