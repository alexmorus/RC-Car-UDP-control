#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <string.h>
#include <Servo.h>

// Set WiFi credentials
#define WIFI_SSID "NodeMCU_Server"
#define WIFI_PASS "12345678"
#define UDP_PORT 4210
#define SERVO 15
#define MOTOR_FWD 14
#define MOTOR_BWD 12

// UDP
WiFiUDP UDP;
char packet[255];
char reply[] = "Pachet primit!";

bool stareFar;

Servo servo;


IPAddress staticIP(192, 168, 4, 173); //ESP static ip
IPAddress gateway(192, 168, 4, 1);   //IP Address of your WiFi Router (Gateway)
IPAddress subnet(255, 255, 255, 0);  //Subnet mask
IPAddress dns(1, 1, 1, 1);


void setup() {
  // Setup serial port
  Serial.begin(9600);
  Serial.println();
  servo.attach(SERVO, 760, 2150);
  servo.write(90);
  pinMode(MOTOR_FWD, OUTPUT);
  pinMode(MOTOR_BWD, OUTPUT);
  analogWrite(MOTOR_FWD, 0);
  analogWrite(MOTOR_BWD, 0);

  WiFi.config(staticIP, subnet, gateway, dns);
  // Begin WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  // Connecting to WiFi...
  Serial.print("Conectare la ");
  Serial.print(WIFI_SSID);
  // Loop continuously while WiFi is not connected
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(100);
    Serial.print(".");
  }

  // Connected to WiFi
  Serial.println();
  Serial.print("Conexiune reusita! Adresa IP: ");
  Serial.println(WiFi.localIP());

  // Begin listening to UDP port
  UDP.begin(UDP_PORT);
  Serial.print("Conectat la portul ");
  Serial.println(UDP_PORT);

}


void loop() {

  // If packet received...
  int packetSize = UDP.parsePacket();
  if (packetSize) {
    Serial.println("Pachet receptionat!");
    int len = UDP.read(packet, 255);
    if (len > 0)
    {
      packet[len] = '\0';
    }
//    Serial.print("Packet received: ");
//    Serial.println(packet);
    if (strcmp(packet, "far\0") == 0) {
      if (!stareFar) {
        Serial.println("Aprindem farurile!");
        //aprindeFar(pin);
        stareFar = true;
      }
      else if (stareFar){
        Serial.println("Stingem farurile!");
        //stingeFar(pin);
        stareFar = false;
      }
    }
    if (strcmp(packet, "far\0") != 0) {

      Serial.println("Control Motor!");
      Serial.println(packet);
      char *pch;
      pch = strtok (packet,"-"); // impart pachetul in 2, viraj si acceleratie, intre 0-255; 95-110 si 100-110 sunt zone tampon(fara actiune)
      int accel = atoi(pch);
      pch = strtok (NULL, "-");
      int viraj = atoi(pch);
      if (accel > 95 && accel < 110) {
          analogWrite(MOTOR_FWD, 0);// daca joystickul este neutru oprim deplasarea in orice directie;
          analogWrite(MOTOR_BWD, 0);
      } else if (accel < 95) {
          analogWrite(MOTOR_BWD, 0);
          analogWrite(MOTOR_FWD, 255 - map(accel, 0, 95, 0, 255));
      } else if (accel > 110) {
          analogWrite(MOTOR_FWD, 0);
          analogWrite(MOTOR_BWD, map(accel, 110, 255, 0, 255));
      }

      if (viraj > 100 && viraj < 110) {
          servo.write(90);
      } else if (viraj < 100) {
          servo.write(map(viraj, 0, 100, 0, 90));
      } else if (viraj > 110) {
          servo.write(90 + map(viraj, 110, 255, 0, 89));
      }
    }
    delay(5);
   }
}
