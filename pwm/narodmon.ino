bool Narodmon() { // Собственно формирование пакета и отправка.
  WiFiClient client;
  String buf;
   DeviceAddress tempDeviceAddress;
  buf = "#" + WiFi.macAddress() + "#" + Hostname + "\r\n"; // заголовок
  int deviceCount = sensors.getDeviceCount();  // узнаем количество подключенных градусников
  sensors.requestTemperatures();

  if (DHT_EN) {
    buf += "#T1#";
    buf += (float)dht.readTemperature();
    buf += "\n";
    buf += "#H1#";
    buf += (float)dht.readHumidity();
    buf += "\n";
  }
  if (BMP_EN) {
    buf += "#T2#";
    buf += bmp.readTemperature();
    buf += "\n";
    buf += "#P1#";
    buf += bmp.readPressure() / 133.3;
    buf += "\n";
  }
  if (DS_EN) {
    for (int i = 0; i < deviceCount; i++)  { //перечисляем датчики и их показания
      sensors.getAddress(tempDeviceAddress, i);
      buf += + "#";
      for (uint8_t i = 0; i < 8; i++) {
        if (tempDeviceAddress[i] < 16) buf += "0";  // адрес датчика
        buf += String(tempDeviceAddress[i], HEX);
      }
      buf += "#" + String(sensors.getTempCByIndex(i)) + "\n"; //и температура
    }
  }
  buf += "##\r\n";
  if (!client.connect("narodmon.ru", 8283)) { // попытка подключения
    Serial.println("connection failed");
    return false; // не удалось;
  } else {
    client.print(buf); // и отправляем данные
    
    while (client.available()) {
      String line = client.readStringUntil('\r'); // если что-то в ответ будет - все в Serial
      Serial.print(line);
    }
  }
  history();
  return true; //ушло
 }



