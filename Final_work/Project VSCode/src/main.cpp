#include <WiFi.h>
#include <Wire.h>
#include <PubSubClient.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_AM2320.h>
#include <Adafruit_SSD1306.h>

#define I2C_display 0x3C
#define I2C_BMP280 0x76

const char* ssid = "TP-Link_6D83";
const char* password = "98519569";
const char* mqtt_server = "138.124.31.84";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

float temperature = 0;
float pressure = 0;
float humidity =0;

Adafruit_SSD1306 display = Adafruit_SSD1306(128, 64, &Wire, -1);
Adafruit_BMP280 bmp;
Adafruit_AM2320 am2320 = Adafruit_AM2320();

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      client.subscribe("esp32/output");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(9600);
  Wire.begin();
  am2320.begin();

  display.begin(SSD1306_SWITCHCAPVCC, I2C_display);
  display.clearDisplay();
  display.setTextColor(WHITE);

  bmp.begin(I2C_BMP280);
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */

  setup_wifi();
  client.setServer(mqtt_server, 1883);
}

void loop() {
  temperature = bmp.readTemperature();
  pressure = bmp.readPressure()*0.00750062;
  humidity = am2320.readHumidity();

  display.clearDisplay();
  display.setCursor(0,0);
  display.setTextSize(1);
  display.println(ssid);
  display.println(WiFi.localIP());
  display.setTextSize(2);
  display.print("T - ");
  display.println(temperature);
  display.print("P - ");
  display.println(pressure);
  display.print("H - ");
  display.println(humidity);
  display.display();

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 5000) {
    lastMsg = now;
    char tempString[8];
    dtostrf(temperature, 1, 2, tempString);
    client.publish("esp32/temperature", tempString);
    char pressString[8];
    dtostrf(pressure, 1, 2, pressString);
    client.publish("esp32/pressure", pressString);
    char humidString[8];
    dtostrf(humidity, 1, 2, humidString);
    client.publish("esp32/humidity", humidString);
  }
}