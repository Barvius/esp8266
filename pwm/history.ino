float History[12][10];
time_t Time_h[12];

void push_history(int row, float val) {
  for (int i = 0; i < 12; i++) {
    History[i][row] = History[i+1][row];

  }
  History[11][row] = val;
  
}

void history() { // Собственно формирование пакета и отправка.
  timeSynch(Timezone);
  if (DHT_EN) {
    push_history(0, (float)dht.readTemperature());
    push_history(1, (float)dht.readHumidity());
  }
  if (BMP_EN) {
    push_history(2, bmp.readTemperature());
    push_history(3, bmp.readPressure() / 133.3);
  }
  if (DS_EN) {
    int deviceCount = sensors.getDeviceCount();  // узнаем количество подключенных градусников
    sensors.requestTemperatures();
    DeviceAddress tempDeviceAddress;
    for (int i = 0; i < deviceCount; i++)  {
      sensors.getAddress(tempDeviceAddress, i);
      push_history(4 + i, sensors.getTempCByIndex(i));
    }
  }
  for (int i = 0; i < 12; i++) {
    Time_h[i] = Time_h[i + 1];
  }
  Time_h[11] = GetUnixTime();
}



