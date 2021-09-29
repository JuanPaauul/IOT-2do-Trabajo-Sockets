#include <WiFi.h>
#define LED_GREEN 14
#define LED_YELLOW 12
#define LED_RED 13
#define TRIGGER_PIN 27
#define ECHO_PIN 26
const char* WIFI_SSID="EGN9265";
const char* WIFI_PASS="5518UA19GR73ALD";
const char* SERVER_ADDRESS="192.168.0.7";
const int SERIAL_PORT =6400;
int cm = 0;
long readUltrasonicDistance(int triggerPin, int echoPin) {
  pinMode(triggerPin, OUTPUT);
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  pinMode(echoPin, INPUT);
  return pulseIn(echoPin, HIGH);
}

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_YELLOW, LOW);
  digitalWrite(LED_GREEN, LOW);
  Serial.begin(115200);
  Serial.print("Conecting to: EGN9265");

  WiFi.begin(WIFI_SSID,WIFI_PASS);
  while(WiFi.status()!=WL_CONNECTED){
      delay(500);
      Serial.print('.');  
  }
  Serial.print("Local ip addres: ");
  Serial.print(WiFi.localIP());
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(500);
  Serial.print("Conecting to: ");
  Serial.print(SERVER_ADDRESS);
  Serial.print("\n");
  
  WiFiClient client;
  
  if(!client.connect(SERVER_ADDRESS, SERIAL_PORT)){
     Serial.print("Conect faild \n");
     delay(1000);
     return;
   }
   char line;
   String rojo = "1";
   do{
    //Serial.println("Hola desde el do while");
      if(client.available()){
        cm = 0.01723 * readUltrasonicDistance(TRIGGER_PIN, ECHO_PIN);
        line= client.read();
        if (line=='1') {
          digitalWrite(LED_RED, HIGH);
          digitalWrite(LED_YELLOW, LOW);
          digitalWrite(LED_GREEN, LOW);
        }
        if (line=='2') {
          digitalWrite(LED_YELLOW, HIGH);
          digitalWrite(LED_RED, LOW);
          digitalWrite(LED_GREEN, LOW);
        }
        if (line=='3') {
          digitalWrite(LED_GREEN, HIGH);
           digitalWrite(LED_RED, LOW);
           digitalWrite(LED_YELLOW, LOW);
        }
        if(line == '5'){
          String mesage= "Dist: ";
          mesage.concat(cm);
          client.println(mesage);
        }
      }
   }while(line!='0');
   client.println("ESP32 dice: Adios :)");
   client.stop();
   delay(500);
}
