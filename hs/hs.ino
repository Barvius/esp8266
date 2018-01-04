#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager
#include <FS.h>
#include <ArduinoJson.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#include <PubSubClient.h>
#include <time.h>

#define PORT 4

int GpioList[] = {4, 5, 13, 12};
int GpioLevel[] = {HIGH, LOW};

String M_Server;
int M_Port;
String M_User;
String M_Password;

String GpioTopics[PORT];
String GpioDescription[PORT];

//ESP8266HTTPUpdateServer httpUpdater;
// Web интерфейс для устройства
ESP8266WebServer HTTP(80);

WiFiClient wclient;
PubSubClient client(wclient);
// Для файловой системы
File fsUploadFile;

void mqttConnect(){
   if (WiFi.status() == WL_CONNECTED) {
    if (!client.connected()) {

      if (client.connect(MQTT::Connect(WiFi.macAddress()).set_auth(M_User, M_Password))) {
        Serial.println("Connected to MQTT server ");
        for (int i = 0; i < PORT; i++) {
          client.subscribe(String("IO/"+GpioTopics[i]+"/value"));
        }
         client.subscribe("Debug");
      } else {
        Serial.println("Could not connect to MQTT server");
      }
    }
  }

  String Sys = WiFi.macAddress();
  Sys += " -- ";
  Sys += WiFi.localIP().toString();
  Sys += " -- (";
  Sys += WiFi.RSSI();
  Sys += ")";
  client.publish("Info", Sys);
  for (int i = 0; i < PORT; i++) {
    client.publish(String("IO/"+GpioTopics[i]+"/value"), String(GpioLevel[digitalRead(GpioList[i])]));
  }
}

void setup() {
  // Настраиваем вывод отладки
  Serial.begin(115200);

  //Включаем WiFiManager
  WiFiManager wifiManager;

  //Если не удалось подключиться клиентом запускаем режим AP
  // доступ к настройкам по адресу http://192.168.4.1
  wifiManager.autoConnect("AutoConnectAP");

  //если подключение к точке доступа произошло сообщаем
  Serial.println("connected...yeey :)");

  for (int i = 0; i < PORT; i++) {
    pinMode(GpioList[i], OUTPUT);
    digitalWrite(GpioList[i], HIGH);
  }

  FS_init();
  loadConfig();

  client.set_server(M_Server, M_Port);
  client.set_callback(callback);

  //WiFi.hostname(Hostname);

  HTTP_init();


}


unsigned long timer = 0;

void refreshData() {
  //if (millis() - timer > 5000) {
  //  for (int i = 0; i < PORT; i++) {
  //    client.publish(GpioTopics[i], String(GpioLevel[digitalRead(GpioList[i])]));
  //  }
  //  timer=millis();
  //}
}

// Функция получения данных от сервера
//void callback(const MQTT::Publish& pub)
void callback(const MQTT::Publish& pub)
{
  String payload = pub.payload_string();
  String topic = pub.topic();

  Serial.print(pub.topic()); // выводим в сериал порт название топика
  Serial.print(" => ");
  Serial.println(payload); // выводим в сериал порт значение полученных данных
  if(topic == "Debug" && payload == "GetNames"){
      for (int i = 0; i < PORT; i++) {
    client.publish(String("IO/"+GpioTopics[i]+"/name"), GpioDescription[i]);
      }
  }
   if(topic == "Debug" && payload == "GetValues"){
      for (int i = 0; i < PORT; i++) {
    client.publish(String("IO/"+GpioTopics[i]+"/value"), String(GpioLevel[digitalRead(GpioList[i])]));
      }
  }
  // проверяем из нужного ли нам топика пришли данные
  for (int i = 0; i < PORT; i++) {
    if (topic == String("IO/"+GpioTopics[i]+"/value")) {
      digitalWrite(GpioList[i], GpioLevel[payload.toInt()]);
    }
  }
}

void loop() {

  HTTP.handleClient();
  delay(1);
  if (client.connected()) {
    client.loop();
    refreshData();
  } else {
    mqttConnect();
  }


}
