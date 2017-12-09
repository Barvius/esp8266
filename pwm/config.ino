String jsonConfig = "{}";
bool loadConfig() {
  // Открываем файл для чтения
  File configFile = SPIFFS.open("/config.json", "r");
  if (!configFile) {
    // если файл не найден
    Serial.println("Failed to open config file");
    //  Создаем файл запиав в него аные по умолчанию
    saveConfig();
    return false;
  }
  // Проверяем размер файла, будем использовать файл размером меньше 1024 байта
  size_t size = configFile.size();
  if (size > 1024) {
    Serial.println("Config file size is too large");
    return false;
  }

  // загружаем файл конфигурации в глобальную переменную
  jsonConfig = configFile.readString();
  // Резервируем памяь для json обекта буфер может рости по мере необходимти предпочтительно для ESP8266
  DynamicJsonBuffer jsonBuffer;
  //  вызовите парсер JSON через экземпляр jsonBuffer
  //  строку возьмем из глобальной переменной String jsonConfig
  JsonObject& root = jsonBuffer.parseObject(jsonConfig);
  // Теперь можно получить значения из root
  //timezone = root["timezone"];               // Так получаем число
  //_ssid = root["ssidName"].as<String>();
  //_password = root["ssidPassword"].as<String>();
  Hostname = root["hostname"].as<String>();
  DS_EN = root["ds_en"];
  DHT_EN = root["dht_en"];
  BMP_EN = root["bmp_en"];
  NM_EN = root["nm_en"];
  NM_INTERVAL = root["nm_interval"];
  Timezone = root["timezone"];
  //  DHT_PIN = root["dht_pin"];
  //  DS18B20_PIN = root["ds18b20_pin"];
  //  if (DS18B20_PIN) {
  //    OneWire oneWire(DS18B20_PIN);
  //    DallasTemperature sensors(&oneWire);
  //  }
  //  if(DHT_PIN){
  //    DHT dht(DHT_PIN, DHT11);
  //  }
  return true;
}

// Запись данных в файл config.json
bool saveConfig() {
  // Резервируем памяь для json обекта буфер может рости по мере необходимти предпочтительно для ESP8266
  DynamicJsonBuffer jsonBuffer;
  //  вызовите парсер JSON через экземпляр jsonBuffer
  JsonObject& json = jsonBuffer.parseObject(jsonConfig);
  // Заполняем поля json
  //json["ssidName"] = _ssid;
  //json["ssidPassword"] = _password;
  //json["timezone"] = timezone;
  json["hostname"] = Hostname;
  json["ds_en"] = DS_EN;
  json["dht_en"] = DHT_EN;
  json["bmp_en"] = BMP_EN;
  json["nm_en"] = NM_EN;
  json["nm_interval"] = NM_INTERVAL;
  json["timezone"] = Timezone;
  // Помещаем созданный json в глобальную переменную json.printTo(jsonConfig);
  json.printTo(jsonConfig);
  // Открываем файл для записи
  File configFile = SPIFFS.open("/config.json", "w");
  if (!configFile) {
    //Serial.println("Failed to open config file for writing");
    return false;
  }
  // Записываем строку json в файл
  json.printTo(configFile);
  return true;
}
