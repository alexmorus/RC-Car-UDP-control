#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <string.h>
#include <Servo.h>

// Informatii WiFi
#define WIFI_SSID "*********"
#define WIFI_PASS "*********"
#define UDP_PORT 4210
#define SERVO 15
#define MOTOR_FWD 14
#define MOTOR_BWD 12
#define echo 2
#define trig 3


// UDP
WiFiUDP UDP;
char packet[255];
char reply[] = "Pachet primit!";

bool stareFar;
int unghi,acceleratie;

Servo servo;

void WiFi_setup(){
  // Setup serial port
  Serial.begin(115200);
  Serial.println();
  servo.attach(SERVO);
  servo.write(90);
  pinMode(MOTOR_FWD, OUTPUT);
  pinMode(MOTOR_BWD, OUTPUT);
  pinMode(trig,OUTPUT);
  pinMode(echo,INPUT);
  analogWrite(MOTOR_FWD, 0);
  analogWrite(MOTOR_BWD, 0);
  // Begin WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  // Conectare la WiFi
  Serial.print("Conectare la ");
  Serial.print(WIFI_SSID);
  // Stam in bucla pana cand se conecteaza la WiFi
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(100);
    Serial.print(".");
  }

  // Conectat
  Serial.println();
  Serial.print("Conexiune reusita! Adresa IP: ");
  Serial.println(WiFi.localIP());

  // Ascultare pe portul UDP
  UDP.begin(UDP_PORT);
  Serial.print("Conectat la portul ");
  Serial.println(UDP_PORT);

}


void controlFar()
{
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
}

void controlMotor()
{


      Serial.println("Control Motor!");
      char *pch;
      pch = strtok (packet,"-"); // impart pachetul in 2, unghi si acceleratie;
      unghi = atoi(pch);
      pch = strtok (NULL, "-");
      acceleratie = atoi(pch);
      Serial.print("Acceleratie: ");
      Serial.print(acceleratie);

      if (unghi == 0 && acceleratie == 0) {
        Serial.println("-> Stationam!");
        analogWrite(MOTOR_FWD, 0);
        analogWrite(MOTOR_BWD, 0);
      }
      if (unghi >= 270 && unghi <= 360 || unghi > 0 && unghi <= 90){
        Serial.println("% in fata");
        analogWrite(MOTOR_BWD, 0); //daca mergem in fata oprim accelerarea in spate
        analogWrite(MOTOR_FWD, acceleratie*10);
      }
      if (unghi < 270 && unghi > 90) {
        Serial.println("% in spate");
        analogWrite(MOTOR_FWD, 0); //daca mergem in spate oprim accelerarea in fata
        analogWrite(MOTOR_BWD, acceleratie*10);
      }
}

void controlServo(){
       Serial.print("Unghi: ");
//      Serial.println(unghi);
       if(unghi >= 0 && unghi <= 90) {
        int val = constrain(unghi+90, 40, 140);
//        servo.write(unghi+90);
        servo.write(val);
        Serial.println(unghi + 90);
      }
      if(unghi <= 360 && unghi > 270) {
        int val = constrain(unghi-270, 40, 140);
//        servo.write(unghi-270);
        servo.write(val);
        Serial.println(unghi - 270);
      }
      if(unghi <= 270 && unghi >= 180) {
        int val = constrain(270 - unghi, 40, 140);
//        servo.write(270-unghi);
        servo.write(val);
        Serial.println(270 - unghi);
      }
      if(unghi < 180 && unghi > 90){
        int val = constrain(270 - unghi, 40, 140);
//        servo.write(270 - unghi);
        servo.write(val);
        Serial.println(270 - unghi);
      }
}

double filtDist(double U)
{
    static const double R=40;
    static const double H=1.00;
    static double Q=10;
    static double P=0;
    static double U_hat=0;
    static double K=0;
    K=P*H/(H*P*H+R);
    U_hat+=K*(U-H*U_hat);
    P=(1-K*H)*P+Q;
    return U_hat;

}

double unfiltDist()
{
  digitalWrite(trig,LOW);
  delayMicroseconds(2);
  digitalWrite(trig,HIGH);
  delayMicroseconds(10);
  digitalWrite(trig,LOW);
  double duration=pulseIn(echo,HIGH);
  double distance=(duration*0.034)/2;
  //Serial.println(distance);

  if (distance>450)
    return 450;
  else if (distance<2)
    return 2;
  else
    return distance;
}

void setup() {
  WiFi_setup();

}

const unsigned long event=200;
unsigned long prev=0;

void loop() {


    //unsigned long current=millis();

     int packetSize = UDP.parsePacket();
    if (packetSize) {
      Serial.println("Pachet receptionat!");
      int len = UDP.read(packet, 255);
      if (len > 0)
      {
        packet[len] = '\0';
      }
      controlFar();
        if (strcmp(packet, "far\0") != 0){
          controlMotor();
          controlServo();
        }
    }
// future support for rear mounted ultrasonic distance sensor
//    if (millis()-prev>event){
//      prev=millis();
//      float dist=filtDist(unfiltDist());
//      if (dist<20){
//        analogWrite(MOTOR_BWD,0);
//        analogWrite(MOTOR_FWD,0);
//        delay(1000);
//      }
//    }

}
//            0
//
//     45           315
//
//  90                 270
//
//    135           225
//
//           180
