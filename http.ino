void HTTP_init(void) {
  HTTP.on("/restart", restart);
  HTTP.on("/system_info", system_info_handler);
  HTTP.on("/available_networks", available_networks_handler);
 HTTP.on("/io", gpio_handler);
 HTTP.on("/mqtt", config_mqtt_handler);
  update();
  HTTP.begin();
}

void config_mqtt_handler() {
  if (HTTP.argName(0) == "server" && HTTP.argName(1) == "port" && HTTP.argName(2) == "user" && HTTP.argName(3) == "password") {
    M_Server = HTTP.arg("server");
    M_Port = HTTP.arg("port").toInt();
    M_User = HTTP.arg("user");
    M_Password = HTTP.arg("password");
    saveConfig();
    HTTP.send(200, "text/plain", "OK"); // отправляем ответ о выполнении
  }
  if (HTTP.argName(0) == "Index" && HTTP.argName(1) == "TopicNum" && HTTP.argName(2) == "TopicDescription") {
    GpioTopics[HTTP.arg("Index").toInt()]= HTTP.arg("TopicNum");
    GpioDescription[HTTP.arg("Index").toInt()]= HTTP.arg("TopicDescription");
    saveConfig();
    HTTP.send(200, "text/plain", "OK"); // отправляем ответ о выполнении
  }
  //HTTP.send(404, "text/plain", "ERR"); // отправляем ответ о выполнении
}

void gpio_handler() {
  if (HTTP.args() == 0) {
      String json = "[";
     for(int i=0; i<4;i++){
       json += "[";
      json += GpioLevel[digitalRead(GpioList[i])];
      json += ",\"";
     // json += GpioDescription[i];
      
      if(i != 3){
      json += "\"],";
      }else {
          json += "\"]";
      }
     }
      
      json += "]";
  HTTP.send(200, "text/json", json);
   
  }
  if (HTTP.argName(0) == "set" && HTTP.argName(1) == "val") {
     digitalWrite(GpioList[HTTP.arg("set").toInt()], GpioLevel[HTTP.arg("val").toInt()]);
    HTTP.send(200); // отправляем ответ о выполнении
  }
}

void available_networks_handler() {
  String json = "[";
  int n = WiFi.scanNetworks();
  if (n) {
    for (int i = 0; i < n; ++i) {
      if (i) {
        json += ",";
      }
      json += "{";
      json += "\"ssid\":\"";
      json += WiFi.SSID(i);
      json += "\",\"rssi\":";
      json += WiFi.RSSI(i);
      json += ",\"encryption\":\"";
      //      json += WiFi.encryptionType(i);
      switch (WiFi.encryptionType(i)) {
        case ENC_TYPE_NONE:
          json += "NONE";
          break;
        case ENC_TYPE_WEP:
          json += "WEP";
          break;
        case ENC_TYPE_TKIP:
          json += "TKIP";
          break;
        case ENC_TYPE_CCMP:
          json += "CCMP";
          break;
        case ENC_TYPE_AUTO:
          json += "AUTO";
          break;
        default:
          json += "?";
          break;
      }
      json += "\"}";
      delay(10);
    }
  }
  json += "]";
  HTTP.send(200, "text/json", json);
}

void restart() {
  HTTP.send(200, "text / plain", "Reset OK");
  ESP.restart();
}

void system_info_handler() {
  String json = "{";
  json += "\"mac\":\"";
  json += WiFi.macAddress();
  json += "\",\"ip\":\"";
  json +=  WiFi.localIP().toString();
  json += "\",\"ssid\":\"";
  json += WiFi.SSID();
  json += "\",\"rssi\":";
  json += WiFi.RSSI();
  json += ",\"uptime\":";
  json += millis();
  json += ",\"vcc\":";
  json +=  ESP.getVcc();
  json += "}";
  HTTP.send(200, "text/json", json);
}
/*
void pwm_handler() {
  if (HTTP.argName(0) == "add") {
    pinMode(HTTP.arg("add").toInt(), OUTPUT);
    HTTP.send(200); // отправляем ответ о выполнении
  }
  if (HTTP.argName(0) == "set" && HTTP.argName(1) == "val") {
     analogWrite(HTTP.arg("set").toInt(), HTTP.arg("val").toInt());
    HTTP.send(200); // отправляем ответ о выполнении
  }
}

void servo_handler() {
  if (HTTP.argName(0) == "deg") {
    
    myservo.write(HTTP.arg("deg").toInt());
    HTTP.send(200); // отправляем ответ о выполнении

  }
}

void history_handler() {
  String json = "{";
  json += "\"time\" : [";
  for (int i = 0; i < 12; i++) {
    json += "\"";
    json += GetTime(Time_h[i]);
    json += "\"";
    if (i < 11) {
      json += ",";
    }
  }
  json += "],";
  json += "\"data\" : [";
  for (int i = 0; i < 10; i++) {
    if (i) {
      json += ",[";
    } else {
      json += "[";
    }

    for (int j = 0; j < 12; j++) {
      json += History[j][i];
      if (j < 11) {
        json += ",";
      }
    }

    json += "]";
  }
  json += "]";
  json += "}";
  HTTP.send(200, "text/json", json);
}

void available_networks_handler() {
  String json = "[";
  int n = WiFi.scanNetworks();
  if (n) {
    for (int i = 0; i < n; ++i) {
      if (i) {
        json += ",";
      }
      json += "{";
      json += "\"ssid\":\"";
      json += WiFi.SSID(i);
      json += "\",\"rssi\":";
      json += WiFi.RSSI(i);
      json += ",\"encryption\":\"";
      //      json += WiFi.encryptionType(i);
      switch (WiFi.encryptionType(i)) {
        case ENC_TYPE_NONE:
          json += "NONE";
          break;
        case ENC_TYPE_WEP:
          json += "WEP";
          break;
        case ENC_TYPE_TKIP:
          json += "TKIP";
          break;
        case ENC_TYPE_CCMP:
          json += "CCMP";
          break;
        case ENC_TYPE_AUTO:
          json += "AUTO";
          break;
        default:
          json += "?";
          break;
      }
      json += "\"}";
      delay(10);
    }
  }
  json += "]";
  HTTP.send(200, "text/json", json);
}

void restart() {
  HTTP.send(200, "text / plain", "Reset OK");
  ESP.restart();
}

void system_info_handler() {
  String json = "{";
  json += "\"mac\":\"";
  json += WiFi.macAddress();
  json += "\",\"ip\":\"";
  json +=  WiFi.localIP().toString();
  json += "\",\"ssid\":\"";
  json += WiFi.SSID();
  json += "\",\"rssi\":";
  json += WiFi.RSSI();
  json += ",\"uptime\":";
  json += millis();
  json += ",\"vcc\":";
  json +=  ESP.getVcc();
  json += "}";
  HTTP.send(200, "text/json", json);
}

void hostname_config() {
  if (HTTP.argName(0) == "name") {
    Hostname = HTTP.arg("name");
    saveConfig();
    HTTP.send(200, "text/plain", "OK"); // отправляем ответ о выполнении
    ESP.restart();
  }
}
void ds_config() {
  if (HTTP.argName(0) == "en") {
    DS_EN = HTTP.arg("en").toInt();
    saveConfig();
    HTTP.send(200, "text/plain", "OK"); // отправляем ответ о выполнении
  }
}
void dht_config() {
  if (HTTP.argName(0) == "en") {
    DHT_EN = HTTP.arg("en").toInt();
    saveConfig();
    HTTP.send(200, "text/plain", "OK"); // отправляем ответ о выполнении
  }
}
void bmp_config() {
  if (HTTP.argName(0) == "en") {
    BMP_EN = HTTP.arg("en").toInt();
    saveConfig();
    HTTP.send(200, "text/plain", "OK"); // отправляем ответ о выполнении
  }
}

void nm_config() {
  if (HTTP.argName(0) == "en") {
    NM_EN = HTTP.arg("en").toInt();
    saveConfig();
    HTTP.send(200, "text/plain", "OK"); // отправляем ответ о выполнении
  }
  if (HTTP.argName(0) == "interval") {
    NM_INTERVAL = HTTP.arg("interval").toInt();
    saveConfig();
    HTTP.send(200, "text/plain", "OK"); // отправляем ответ о выполнении
  }
}
*/
//void handle_Set_DS18b20_Pin() {
//  DS18B20_PIN = HTTP.arg("pin").toInt(); // Получаем значение ssdp из запроса сохраняем в глобальной переменной
//  saveConfig();                 // Функция сохранения данных во Flash пока пустая
//  HTTP.send(200, "text/plain", "OK"); // отправляем ответ о выполнении
//}
