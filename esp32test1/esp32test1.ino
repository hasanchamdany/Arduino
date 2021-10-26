// Example testing sketch for various DHT humidity/temperature sensors written by ladyada
// REQUIRES the following Arduino libraries:
// - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library
// - Adafruit Unified Sensor Lib: https://github.com/adafruit/Adafruit_Sensor

#include "DHT.h"

#define DHTPIN 4     // Digital pin connected to the DHT sensor
// Feather HUZZAH ESP8266 note: use pins 3, 4, 5, 12, 13 or 14 --
// Pin 15 can work but DHT must be disconnected during program upload.

// Uncomment whatever type you're using!
//#define DHTTYPE DHT11   // DHT 11
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)


#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "Error404";
const char* password = "findtheflag";
/* this is the IP of PC/raspberry where you installed MQTT Server 
on Wins use "ipconfig" 
on Linux use "ifconfig" to get its IP address */
const char* mqtt_server = "192.168.43.205";
float temperature = 0;

/* create an instance of PubSubClient client */
WiFiClient espClient;
PubSubClient client(espClient);

/*LED GPIO pin*/
const char led = 4;

/* topics */
#define TEMP_TOPIC    "floor1/temp"
#define LED_TOPIC     "floor1/led" /* true=on, false=off */

long lastMsg = 0;
char msg[20];



DHT dht(DHTPIN, DHTTYPE);


void receivedCallback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message received: ");
  Serial.println(topic);

  Serial.print("payload: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  /* we got '1' -> on */
  if ((char)payload[0] == 't') {
    digitalWrite(led, HIGH); 
  } else {
    /* we got '0' -> on */
    digitalWrite(led, LOW);
  }

}

void mqttconnect() {
  /* Loop until reconnected */
  while (!client.connected()) {
    Serial.print("MQTT connecting ...");
    /* client ID */
    String clientId = "ESP32Client";
    /* connect now */
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      /* subscribe topic with default QoS 0*/
      client.subscribe(LED_TOPIC);
    } else {
      Serial.print("failed, status code =");
      Serial.print(client.state());
      Serial.println("try again in 5 seconds");
      /* Wait 5 seconds before retrying */
      delay(5000);
    }
  }
}



















void setup() {
  Serial.begin(9600);
  Serial.println(F("DHTxx test!"));

   Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  /* set led as output to control led on-off */
  pinMode(led, OUTPUT);

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  /* configure the MQTT server with IPaddress and port */
  client.setServer(mqtt_server, 1883);
  /* this receivedCallback function will be invoked 
  when client received subscribed topic */
  client.setCallback(receivedCallback);

  

  dht.begin();
}

void loop() {

  if(client.connected()) {
    mqttconnect();
  }

  client.loop();
  // Wait a few seconds between measurements.
  delay(2000);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  //  float temperature = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(temperature) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(temperature, h, false);


  long now = millis();
  if (now - lastMsg > 3000) {
    lastMsg = now;
    temperature = dht.readTemperature();
    if (!isnan(temperature)) {
      snprintf (msg, 20, "%lf", temperature);
      /* publish the message */
      client.publish(TEMP_TOPIC, msg);
    }
  }
  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(temperature);
  Serial.print(F("°C "));
  Serial.print(f);
  Serial.print(F("°F  Heat index: "));
  Serial.print(hic);
  Serial.print(F("°C "));
  Serial.print(hif);
  Serial.println(F("°F"));
}
