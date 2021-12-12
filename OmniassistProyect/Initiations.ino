void initWiFi() {
  Serial.println("Conectando a la red WIFI");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Conectado a la red WIFI");
}

void initTB() {
  Serial.print("Conectando al servidor de ThingsBoard: ");
  Serial.print(THINGSBOARD_SERVER);
  Serial.print(" con el  token ");
  Serial.println(TOKEN);
  while (!tb.connect(THINGSBOARD_SERVER, TOKEN)) {
    delay(500);
    Serial.print(".");
  }
}

void initTime() {
  struct tm timeinfo;
  if (WiFi.status() != WL_CONNECTED) {
    initWiFi();
  }
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  if (getLocalTime(&timeinfo)) {
    rtc.setTimeStruct(timeinfo);
  }
}

void initAcc() {
  if (!mma.begin()) {
    Serial.println("No se pudo iniciar el acelerómetro MMA8451");
  } else {
    Serial.println("MMA8451 encontrado");
    pinMode(buzzer, OUTPUT);
    mma.setRange(MMA8451_RANGE_2_G);
    Serial.print("Range = "); Serial.print(2 << mma.getRange());
    Serial.println("G");
  }
}

void initDisplay() {
  if ( ! display.begin(0x3C) ) {
    Serial.println("No se ha encontrado el display.");
  } else Serial.println("Display iniciado.");
  display.fillRect(0, 0, 128, 128, SSD1327_BLACK); //Para no tener que limpiar toda la pantalla
  display.display();
  //  display.fillRect(64,0,2,128,SSD1327_WHITE); //para saber donde está el centro del display
  //  display.fillRect(0,64,128,2,SSD1327_WHITE);
  display.setTextSize(1);
  display.setTextColor(SSD1327_WHITE);
  display.setFont(&FreeSerifBold9pt7b);
  display.setCursor(15, 20);
  display.print("INICIANDO");
  display.drawBitmap(25, 25, logoInicio_bitmap , 80, 80, SSD1327_WHITE);
  display.setCursor(10, 120);
  display.print("OMNIASSIST");
  display.display();
}

void initConfig() {
  EEPROM.begin(EEPROM_SIZE);
  EepromStream eepromStream(0, 1536);
//      for (int i =0 ; i < 1536; i++) {
//        EEPROM.write(i, 0);
//      }
//      EEPROM.commit();
//      delay(500);
  DeserializationError error = deserializeJson(confJSON, eepromStream);
  if (error) {
    Serial.println("No hay JSON de configuracion");
    createEmptyJson();
  }
  if (!confJSON["configured"]) {
    startConfig();
  }
}
