

void raw() {

  int deviceCount = sensors.getDeviceCount();  // узнаем количество подключенных градусников
  sensors.requestTemperatures();
 
  HTTP.sendHeader("Connection", "close");
  HTTP.sendHeader("Access-Control-Allow-Origin", "*");
  bool pre = false;
  String json = "{";
  if (DHT_EN) {
    pre = true;
    json += "\"dht\":{";
    json += "\"t\":";
    json += (float)dht.readTemperature();
    json += ",\"h\":";
    json += (float)dht.readHumidity();
    json += "}";
  }
  if (BMP_EN) {
    if (pre) {
      json += ",\"bmp\":{";
    } else {
      json += "\"bmp\":{";
    }
    pre = true;
    json += "\"t\":";
    json += bmp.readTemperature();
    json += ",\"p\":";
    json += bmp.readPressure() / 133.3;
    json += "}";
  } 
  if (DS_EN) {
    for (int i = 0; i <= deviceCount - 1; i++) {
      DeviceAddress Address18b20;
      sensors.getAddress(Address18b20, i);
      if (pre) {
        json += ",\"ds_";
      } else {
        json += "\"ds_";
      }
      pre = true;
      json += i;
      json += "\":{";
      json += "\"t\":";
      json += sensors.getTempC(Address18b20);

      json += "}";
    }
  }
  json += "}";


  HTTP.send(200, "text/json", json);

}

