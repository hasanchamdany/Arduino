const int relay1 = 26; //pin2
const int relay2 = 27; //pin3
const int relay3 = 14; //pin4
const int relay4 = 12; //pin5
int relayON = LOW; //relay nyala
int relayOFF = HIGH; //relay mati
void setup() {
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  pinMode(relay4, OUTPUT);
  digitalWrite(relay1, relayOFF);
  digitalWrite(relay2, relayOFF);
  digitalWrite(relay3, relayOFF);
  digitalWrite(relay4, relayOFF);
}
void loop() {
  //relay1
  digitalWrite(relay1, relayON);
  delay(1000);
  digitalWrite(relay1, relayOFF);
  delay(1000);
  //relay2
  digitalWrite(relay2, relayON);
  delay(1000);
  digitalWrite(relay2, relayOFF);
  delay(1000);
  //relay3
  digitalWrite(relay3, relayON);
  delay(1000);
  digitalWrite(relay3, relayOFF);
  delay(1000);
  //relay4
  digitalWrite(relay4, relayON);
  delay(1000);
  digitalWrite(relay4, relayOFF);
  delay(1000);
}
