/*
 Basic ESP8266 MQTT example
 This sketch demonstrates the capabilities of the pubsub library in combination
 with the ESP8266 board/library.
 It connects to an MQTT server then:
  - publishes "hello world" to the topic "outTopic" every two seconds
  - subscribes to the topic "inTopic", printing out any messages
    it receives. NB - it assumes the received payloads are strings not binary
  - If the first character of the topic "inTopic" is an 1, switch ON the ESP Led,
    else switch it off
 It will reconnect to the server if the connection is lost using a blocking
 reconnect function. See the 'mqtt_reconnect_nonblocking' example for how to
 achieve the same result without blocking the main loop.
 To install the ESP8266 board, (using Arduino 1.6.4+):
  - Add the following 3rd party board manager under "File -> Preferences -> Additional Boards Manager URLs":
       http://arduino.esp8266.com/stable/package_esp8266com_index.json
  - Open the "Tools -> Board -> Board Manager" and click install for the ESP8266"
  - Select your ESP8266 in "Tools -> Board"
*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <string>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

#define COUNT 10 // SD3 use for temporary during no input sensor.
#define LED 2    // D4
#define CLEAR 13 // D7

// Update these with values suitable for your network.

const char *ssid = "Pawan";
const char *password = "a12345678";
const char *mqtt_server = "35.197.147.237";

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE (50)
char msg[MSG_BUFFER_SIZE];
int countValue = 0;
char sCountValue[5];

void setup_wifi()
{

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1')
  {
    digitalWrite(LED, LOW); // Turn the LED on
  }
  else
  {
    digitalWrite(BUILTIN_LED, HIGH); // Turn the LED off
  }
}

void reconnect()
{
  // Loop until we're reconnected
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str()))
    {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("atthana/test", "hello from GCP of Q");
      // ... and resubscribe
      client.subscribe("atthana/test");
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup()
{

  pinMode(COUNT, INPUT);        // D1 = GPIO5
  pinMode(CLEAR, INPUT);        // D2 is GPIO13
  pinMode(LED, OUTPUT);         // LED Builtin D4 = GPIO2
  pinMode(BUILTIN_LED, OUTPUT); // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
  Wire.begin(D2, D1); //Use predefined PINS consts
  lcd.begin(16, 2);   // The begin call takes the width and height. This
                      // Should match the number provided to the constructor.
  lcd.backlight();    // Turn on the backlight.
  lcd.home();
  lcd.setCursor(0, 0); // Move the cursor at origin
  lcd.print("COUNTING SYSTEM");
  lcd.setCursor(6, 1);
  lcd.print("0");

  // Serial.begin(19200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop()
{

  if (!client.connected())
  {
    reconnect();
  }
  client.loop();

  //  unsigned long now = millis();
  //  if (now - lastMsg > 2000) {
  //    lastMsg = now;
  //    ++value;
  //    snprintf (msg, MSG_BUFFER_SIZE, "hello world #%ld", value);
  //    Serial.print("Publish message: ");
  //    Serial.println(msg);
  //    client.publish("outTopic", msg);
  //
  //  }

  //=========== Count up ===========
  if (digitalRead(COUNT) == LOW)
  {
    while (digitalRead(COUNT) == LOW)
    {
    }
    countValue = countValue + 1;
    itoa(countValue, sCountValue, 10);
    Serial.println(countValue);
    lcd.setCursor(6, 1);
    lcd.print(sCountValue);
    snprintf(msg, MSG_BUFFER_SIZE, sCountValue); // Message to MQTT server.
    client.publish("atthana/shout", msg);
    digitalWrite(LED, LOW); // LED Build in = ON
  }

  //====== Clear count value =====
  else if (digitalRead(CLEAR) == LOW)
  {
    while (digitalRead(CLEAR) == LOW)
    {
    }
    countValue = 0;
    itoa(countValue, sCountValue, 10);
    Serial.println(countValue);
    lcd.clear();
    lcd.home();
    lcd.setCursor(0, 0); // Move the cursor at origin
    lcd.print("COUNTING SYSTEM");
    lcd.setCursor(6, 1);
    lcd.print(sCountValue);
    snprintf(msg, MSG_BUFFER_SIZE, sCountValue); // Message to MQTT server.
    client.publish("atthana/shout", msg);
    digitalWrite(LED, LOW); // LED Build in = ON
  }

  else
  {
    digitalWrite(LED, HIGH); // LED Build in == OFF
  }
}


