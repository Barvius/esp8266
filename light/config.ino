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
  M_Server = root["M_Server"].as<String>();
  M_Port = root["M_Port"];
  M_User = root["M_User"].as<String>();
  M_Password = root["M_Password"].as<String>();
  GpioTopics[0] = root["GpioTopics_0"].as<String>();
  GpioTopics[1] = root["GpioTopics_1"].as<String>();
  GpioTopics[2] = root["GpioTopics_2"].as<String>();
  GpioTopics[3] = root["GpioTopics_3"].as<String>();

  return true;
}

// Запись данных в файл config.json
bool saveConfig() {
  // Резервируем памяь для json обекта буфер может рости по мере необходимти предпочтительно для ESP8266
  DynamicJsonBuffer jsonBuffer;

  //  вызовите парсер JSON через экземпляр jsonBuffer
  JsonObject& json = jsonBuffer.parseObject(jsonConfig);
  json["M_Server"] = M_Server;
  json["M_Port"] = M_Port;
  json["M_User"] = M_User;
  json["M_Password"] = M_Password;
  json["GpioTopics_0"] =  GpioTopics[0];
  json["GpioTopics_1"] =  GpioTopics[1];
  json["GpioTopics_2"] =  GpioTopics[2];
  json["GpioTopics_3"] =  GpioTopics[3];

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
