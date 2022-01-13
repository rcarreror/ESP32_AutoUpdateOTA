void drawBitmap() {
  display.setTextSize(1);
  display.setTextColor(SSD1327_WHITE);
  display.setFont(&FreeSerifBold18pt7b);
  display.setCursor(20, 30);
  //display.clearDisplay();
  display.fillRect(0, 0, 128, 40, SSD1327_BLACK); //Para no tener que limpiar toda la pantalla
  display.print(horamin);
  //display.display();
  display.drawBitmap(0, 41, frame_bitmap, 128, 87, SSD1327_WHITE);
  display.setFont(&FreeMonoBold9pt7b);

  drawWifi(WiFi.RSSI());
  //display.drawBitmap(109, 0, dataBaseNo_bitmap, 19, 19, SSD1327_WHITE);


  for (int i = 0; i < 3; i++) {
    int tomasNecesarias = confJSON["pastillas"][i]["tomasNecesarias"].size();
    int tomasRealizadas = confJSON["pastillas"][i]["ultimasTomas"].size();
    if (tomasRealizadas < tomasNecesarias) {
      display.setTextColor(SSD1327_WHITE);
      display.drawBitmap(10 + (i * 42), 46, pill_bitmap, 23, 52, SSD1327_WHITE);
    }
    else {
      display.setTextColor(SSD1327_BLACK);
      display.fillRect(10 + (i * 42), 46, 23, 52, SSD1327_BLACK);
      display.drawBitmap(10 + (i * 42), 46, filledPill_bitmap, 23, 52, SSD1327_WHITE);
    }
    display.setCursor(15 + (i * 42), 63);
    if (i == 0) display.print("A");
    if (i == 1) display.print("B");
    if (i == 2) display.print("C");

    for (int j = 0; j < tomasNecesarias; j++) {
      display.drawBitmap(4 + (i * 42) + (13 * j), 110, square_bitmap, 11, 11, SSD1327_WHITE);
      if (tomasRealizadas >= j + 1) {
        display.drawBitmap(4 + (i * 42) + (13 * j), 110, cross_bitmap, 11, 11, SSD1327_WHITE);
      }
    }
  }
}

void drawWifi(int strength) {
  display.fillRect(0, 0 , 19, 14, SSD1327_BLACK);
  if (strength == -100)  display.drawBitmap(0, 0, noWifi, 19, 14, SSD1327_WHITE);
  else if (strength <= -60) display.drawBitmap(0, 0, wifi1, 19, 14, SSD1327_WHITE);
  else if (strength <= -50) display.drawBitmap(0, 0, wifi2, 19, 14, SSD1327_WHITE);
  else if (strength <= -40) display.drawBitmap(0, 0, wifi3, 19, 14, SSD1327_WHITE);
  else if (strength > -40) display.drawBitmap(0, 0, wifi4, 19, 14, SSD1327_WHITE);
}

//Escribe la fecha y hora actual en la tarjeta NFC
void escribirFecha(char* msg) {
  NdefMessage message = NdefMessage();
  message.addTextRecord(msg);
  bool success = nfc.write(message);
  if (!success) {
    Serial.println("Error al escribir en la tarjeta NFC.");
  }
}


void firmwareUpdate(void) {
  WiFiClientSecure client;
  client.setCACert(rootCACertificate);
  t_httpUpdate_return ret = httpUpdate.update(client, URL_fw_Bin);

  switch (ret) {
    case HTTP_UPDATE_FAILED:
      Serial.printf("HTTP_UPDATE_FAILED Error (%d): %s\n", httpUpdate.getLastError(), httpUpdate.getLastErrorString().c_str());
      break;

    case HTTP_UPDATE_NO_UPDATES:
      Serial.println("HTTP_UPDATE_NO_UPDATES");
      break;

    case HTTP_UPDATE_OK:
      Serial.println("HTTP_UPDATE_OK");
      break;
  }
}


int FirmwareVersionCheck(void) {
  String payload;
  int httpCode;
  String fwurl = "";
  fwurl += URL_fw_Version;
  fwurl += "?";
  fwurl += String(rand());
  Serial.println(fwurl);
  WiFiClientSecure * client = new WiFiClientSecure;

  if (client) {
    client -> setCACert(rootCACertificate);

    // Add a scoping block for HTTPClient https to make sure it is destroyed before WiFiClientSecure *client is
    HTTPClient https;

    if (https.begin( * client, fwurl))
    { // HTTPS
      Serial.print("[HTTPS] GET...\n");
      // start connection and send HTTP header
      //delay(100);
      httpCode = https.GET();
      //delay(100);
      if (httpCode == HTTP_CODE_OK) // if version received
      {
        payload = https.getString(); // save received version
      } else {
        Serial.print("error in downloading version file:");
        Serial.println(httpCode);
      }
      https.end();
    }
    delete client;
  }

  if (httpCode == HTTP_CODE_OK) // if version received
  {
    payload.trim();
    if (payload.equals(FirmwareVer)) {
      Serial.printf("\nDevice already on latest firmware version:%s\n", FirmwareVer);
      return 0;
    }
    else  {
      Serial.println(payload);
      Serial.println("New firmware detected");
      return 1;
    }
  }
  return 0;
}

void pillsTime(int h, int m) {
  int numPastillas = confJSON["pastillas"].size();
  if (today != confJSON["diaActual"] ) { //Si ha cambiado el día, se reinician todas las tomas de pastillas
    confJSON["diaActual"] = today;
    for (int i = 0; i < numPastillas; i++) {
      while ( confJSON["pastillas"][i]["ultimasTomas"].size() > 0) {
        confJSON["pastillas"][i]["ultimasTomas"].remove(0);
      }
    }
  }
  for (int i = 0; i < numPastillas; i++) {
    if (confJSON["pastillas"][i]["ultimasTomas"].size() != 0) { //comprobamos si se ha tomado alguna pastilla
      if (confJSON["pastillas"][i]["tomasNecesarias"][0] <= h && m > 30) {
        if((millis() - lastWarning)>30000 || lastWarning==0){ 
               const char* nombrePastilla = confJSON["pastillas"][i]["nombrePastilla"];
               String aviso= "AVISO: El usuario no se ha tomado la pastilla "+String(nombrePastilla);
              digitalWrite(buzzer, HIGH);
              delay(1000);
              digitalWrite(buzzer, LOW);
              bot.sendMessage( confJSON["idAsistente"] , aviso);
        }
      }
    }
  }
}


//revisa si hay actualizaciones de firmware e inicia la configuración por medio de un bot de Telegam con el cliente
void startConfig() {
  display.fillRect(0, 0, 128, 128, SSD1327_BLACK); //Para no tener que limpiar toda la pantalla
  display.drawBitmap(25, 25, config_Bitmap , 80, 80, SSD1327_WHITE);
  display.display();
  if (FirmwareVersionCheck()) {
    firmwareUpdate();
  }
  
  confJSON["configured"] = false;
  while (!confJSON["configured"]) {
    bot.getUpdates(bot.last_message_received + 1);
    if (bot.messages[0].text == "/start") {
      String chat_id = bot.messages[0].chat_id;
      String from_name = bot.messages[0].from_name;
      if (from_name == "")
        from_name = "Guest";
      String keyboardJson = "[[\"Borrar configuracion y crear una nueva\"],[ \"Crear configuracion de prueba\"], [\"Cancelar\"]]";
      bot.sendMessageWithReplyKeyboard(chat_id, "¿Cómo quiere configurar su dispositivo?", "", keyboardJson, false, true, false);
      while (!bot.getUpdates(bot.last_message_received + 1)) {
        ;
      }
      String text = bot.messages[0].text;
      if (text == "Borrar configuracion y crear una nueva") {
        bot.sendMessage(chat_id, "De acuerdo, " + from_name + ", comencemos a configurar el sistema.");
        createEmptyJson();
        confJSON["idAsistente"] = chat_id;
        bot.sendMessage( confJSON["idAsistente"] , "Escriba el nombre y apellidos del cliente (persona que toma llevará el sistema)");
        while (!bot.getUpdates(bot.last_message_received + 1)) {
          ;
        }
        text = bot.messages[0].text;
        confJSON["nombreCliente"] = text;
        bot.sendMessage( confJSON["idAsistente"] , "Escriba el nombre y apellidos del asistente");
        while (!bot.getUpdates(bot.last_message_received + 1)) {
          ;
        }
        text = bot.messages[0].text;
        confJSON["nombreAsistente"] = text;
        bot.sendMessage( confJSON["idAsistente"] , "A continuación empezaremos a configurar los medicamentos...");
        for (int i = 0; i < 3; i++) {
          bot.sendMessage( confJSON["idAsistente"] , "Escriba el nombre del mediacamento " + String(i+1));
          while (!bot.getUpdates(bot.last_message_received + 1)) {
            ;
          }
          text = bot.messages[0].text;
          confJSON["pastillas"][i]["nombrePastilla"] = text;
          bot.sendMessage( confJSON["idAsistente"] , "Escriba el tipo de mediacamento (antihipertensivo, analgésico, ansiolítico...)");
          while (!bot.getUpdates(bot.last_message_received + 1)) {
            ;
          }
          text = bot.messages[0].text;
          confJSON["pastillas"][i]["tipoPastilla"] = text;
          bot.sendMessage( confJSON["idAsistente"] , "Escriba la frecuencia (cada 8, 12 ó 24 horas)");
          while (!bot.getUpdates(bot.last_message_received + 1)) {
            ;
          }
          text = bot.messages[0].text;
          confJSON["pastillas"][i]["frecuencia"] = text.toInt();
        }
        confJSON["configured"] = true;
      }
      else if (text == "Crear configuracion de prueba") createTestJson();
      else if (text == "Cancelar")  confJSON["configured"] = true;
    }
  }
  const char* nombreAsistente = confJSON["nombreAsistente"];
  const char* nombreCliente = confJSON["nombreCliente"];
  String endConfig = "Enhorabuena, " + String(nombreAsistente) + ", ha configurado el sistema de " + String(nombreCliente);
  bot.sendMessage(confJSON["idAsistente"], endConfig);
  serializeJson(confJSON, eepromStream);
  eepromStream.flush();
  display.fillRect(0, 0, 128, 128, SSD1327_BLACK);
}
