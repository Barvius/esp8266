#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager
#include <FS.h>
#include <ArduinoJson.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <DHT.h>
#include <Adafruit_BMP085.h>
#include <Servo.h> 
 

#define SERVO_PIN 14
#define DHT_PIN 13
#define DS18B20_PIN 12


ADC_MODE(ADC_VCC);

//String _ssid; // Для хранения SSID
//String _password; // Для хранения пароля сети
//String _ssidAP = "WiFi";   // SSID AP точки доступа
//String _passwordAP = ""; // пароль точки доступа
String Hostname;
bool DS_EN;
bool DHT_EN;
bool BMP_EN;
bool NM_EN;
int NM_INTERVAL;
int Timezone;               // часовой пояс GTM

unsigned long LastSendTimer_0 = 0;

OneWire oneWire(DS18B20_PIN);
DallasTemperature sensors(&oneWire);
DHT dht(DHT_PIN, DHT11);
Adafruit_BMP085 bmp;

ESP8266HTTPUpdateServer httpUpdater;
// Web интерфейс для устройства
ESP8266WebServer HTTP(80);
// Для файловой системы
File fsUploadFile;

Servo myservo;
void setup() {
  // Настраиваем вывод отладки
  Serial.begin(115200);

myservo.attach(SERVO_PIN);
myservo.write(0);
//myservo.detach();
  //настраиваем HTTP интерфейс


  FS_init();
  loadConfig();
  WiFi.hostname(Hostname);

  //Включаем WiFiManager
  WiFiManager wifiManager;

  //Если не удалось подключиться клиентом запускаем режим AP
  // доступ к настройкам по адресу http://192.168.4.1
  wifiManager.autoConnect("AutoConnectAP");

  //если подключение к точке доступа произошло сообщаем
  Serial.println("connected...yeey :)");
  HTTP_init();
  Time_init();
  sensors.begin();
  sensors.setResolution(12);
  dht.begin();



  if (!bmp.begin()) {
    Serial.println("Could not find a valid BMP180 sensor, check wiring!");
  }

}





void loop() {

  HTTP.handleClient();
  delay(1);

  if (millis() - LastSendTimer_0 > NM_INTERVAL && NM_EN) {
    if (WiFi.status() == WL_CONNECTED) {
      if (Narodmon()) {
        LastSendTimer_0 = millis();
      } else {
        LastSendTimer_0 = millis() - NM_INTERVAL + 15000;
      }
    } else {
      LastSendTimer_0 = millis() - NM_INTERVAL + 15000;
      Serial.println("Not connected to WiFi");
    }
  }
}
