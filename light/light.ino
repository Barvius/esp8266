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
String GpioDescription[] = {"Свет 1", "Свет 2", "Свет 3", "Свет 4"};

String M_Server;
int M_Port;
String M_User;
String M_Password;

String GpioTopics[PORT];


//ESP8266HTTPUpdateServer httpUpdater;
// Web интерфейс для устройства
ESP8266WebServer HTTP(80);

WiFiClient wclient;
PubSubClient client(wclient);
// Для файловой системы
File fsUploadFile;



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

  if (WiFi.status() == WL_CONNECTED) {
    if (!client.connected()) {

      if (client.connect(MQTT::Connect("arduinoClient2").set_auth(M_User, M_Password))) {
        Serial.println("Connected to MQTT server ");

        // подписываемся под топики
        client.subscribe(GpioTopics[0]);
        client.subscribe(GpioTopics[1]);
        client.subscribe(GpioTopics[2]);
        client.subscribe(GpioTopics[3]);
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
  //for (int i = 0; i < PORT; i++) {
  //  client.publish(GpioTopics[i], String(GpioLevel[digitalRead(GpioList[i])]));
  //}


  //WiFi.hostname(Hostname);

  HTTP_init();


}


int tm = 300;
float temp = 0;

// Функция отправки показаний
void refreshData() {
  if (tm == 0) {
    for (int i = 0; i < PORT; i++) {
      client.publish(GpioTopics[i], String(GpioLevel[digitalRead(GpioList[i])]));
    }
    tm = 5000; // пауза меду отправками 3 секунды
  }
  tm--;

  delay(1);
}

// Функция получения данных от сервера
void callback(const MQTT::Publish& pub)
{
  String payload = pub.payload_string();
  String topic = pub.topic();

  Serial.print(pub.topic()); // выводим в сериал порт название топика
  Serial.print(" => ");
  Serial.println(payload); // выводим в сериал порт значение полученных данных

  // проверяем из нужного ли нам топика пришли данные
  for (int i = 0; i < PORT; i++) {
    if (topic == GpioTopics[i]) {
      Serial.println(GpioTopics[i]); // выводим в сериал порт подтверждение, что мы получили топик test/2
      digitalWrite(GpioList[i], GpioLevel[payload.toInt()]);
      //if(GpioLevel[digitalRead(GpioList[i])] != GpioLevel[payload.toInt()]){
      //   client.publish(topic, payload);
      //}
    }
  }
}

void loop() {

  HTTP.handleClient();
  delay(1);
  if (client.connected()) {
    client.loop();
    refreshData();
  }


}
