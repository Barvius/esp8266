#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager
#include <FS.h>
#include <ArduinoJson.h>
#include <OneWire.h>
#include <DallasTemperature.h>

int GpioList[]={4,5,13,12};
int GpioLevel[]={HIGH,LOW};

ESP8266HTTPUpdateServer httpUpdater;
// Web интерфейс для устройства
ESP8266WebServer HTTP(80);
// Для файловой системы
File fsUploadFile;

void setup() {
  // Настраиваем вывод отладки
  Serial.begin(115200);
 for(int i=0; i<4;i++){
  pinMode(GpioList[i], OUTPUT);
 digitalWrite(GpioList[i], HIGH);
 }
 
  FS_init();
  //loadConfig();
  //WiFi.hostname(Hostname);

  //Включаем WiFiManager
  WiFiManager wifiManager;

  //Если не удалось подключиться клиентом запускаем режим AP
  // доступ к настройкам по адресу http://192.168.4.1
  wifiManager.autoConnect("AutoConnectAP");

  //если подключение к точке доступа произошло сообщаем
  Serial.println("connected...yeey :)");
  HTTP_init();


}





void loop() {

  HTTP.handleClient();
  delay(1);

  
}
