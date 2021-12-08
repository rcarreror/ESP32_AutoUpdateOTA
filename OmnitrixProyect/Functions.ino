void drawBitmap() {
  display.drawBitmap(0, 41, frame_bitmap, 128, 87, SSD1327_WHITE);
  if (pastilla1 < 3) display.drawBitmap(10, 46, pill_bitmap, 23, 52, SSD1327_WHITE);
  if (pastilla2 < 3) display.drawBitmap(52, 46, pill_bitmap, 23, 52, SSD1327_WHITE);
  if (pastilla3 < 3) display.drawBitmap(94, 46, pill_bitmap, 23, 52, SSD1327_WHITE);

  display.drawBitmap(4, 110, square_bitmap, 11, 11, SSD1327_WHITE);
  display.drawBitmap(17, 110, square_bitmap, 11, 11, SSD1327_WHITE);
  display.drawBitmap(30, 110, square_bitmap, 11, 11, SSD1327_WHITE);

  display.drawBitmap(46, 110, square_bitmap, 11, 11, SSD1327_WHITE);
  display.drawBitmap(59, 110, square_bitmap, 11, 11, SSD1327_WHITE);
  display.drawBitmap(72, 110, square_bitmap, 11, 11, SSD1327_WHITE);

  display.drawBitmap(88, 110, square_bitmap, 11, 11, SSD1327_WHITE);
  display.drawBitmap(101, 110, square_bitmap, 11, 11, SSD1327_WHITE);
  display.drawBitmap(114, 110, square_bitmap, 11, 11, SSD1327_WHITE);

  if (pastilla1 == 1) display.drawBitmap(4, 110, cross_bitmap, 11, 11, SSD1327_WHITE);
  if (pastilla1 == 2)  display.drawBitmap(17, 110, cross_bitmap, 11, 11, SSD1327_WHITE);
  if (pastilla1 == 3) {
    display.drawBitmap(30, 110, cross_bitmap, 11, 11, SSD1327_WHITE);
    display.fillRect(10, 46, 23, 52, SSD1327_BLACK);
    display.drawBitmap(10, 46, filledPill_bitmap, 23, 52, SSD1327_WHITE);
  }

  if (pastilla2 == 1) display.drawBitmap(46, 110, cross_bitmap, 11, 11, SSD1327_WHITE);
  if (pastilla2 == 2) display.drawBitmap(59, 110, cross_bitmap, 11, 11, SSD1327_WHITE);
  if (pastilla2 == 3) {
    display.drawBitmap(72, 110, cross_bitmap, 11, 11, SSD1327_WHITE);
    display.fillRect(52, 46, 23, 52, SSD1327_BLACK);
    display.drawBitmap(52, 46, filledPill_bitmap, 23, 52, SSD1327_WHITE);
  }
  if (pastilla3 == 1) display.drawBitmap(88, 110, cross_bitmap, 11, 11, SSD1327_WHITE);
  if (pastilla3 == 2) display.drawBitmap(101, 110, cross_bitmap, 11, 11, SSD1327_WHITE);
  if (pastilla3 == 3) {
    display.drawBitmap(114, 110, cross_bitmap, 11, 11, SSD1327_WHITE);
    display.fillRect(94, 46, 23, 52, SSD1327_BLACK);
    display.drawBitmap(94, 46, filledPill_bitmap, 23, 52, SSD1327_WHITE);
  }
}

void handleNewMessages(int numNewMessages) {
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));

  for (int i = 0; i < numNewMessages; i++)
  {
    String chat_id = bot.messages[i].chat_id;
    String text = bot.messages[i].text;

    String from_name = bot.messages[i].from_name;
    if (from_name == "")
      from_name = "Guest";

    if (text == "/start")
    {
      String welcome = "Welcome to Universal Arduino Telegram Bot library, " + from_name + ".\n";
      welcome += "Vamos a comenzar a configurar el Omnitrix.\n\n";
      welcome += "/send_test_action : \n";
      bot.sendMessage(chat_id, welcome);
    }
  }
}

//Escribe la fecha y hora actual en la tarjeta NFC
void escribirFecha(char* msg) {
  NdefMessage message = NdefMessage();
  message.addTextRecord(msg);
  bool success = nfc.write(message);
  if (success) {
    Serial.println("Fecha actualizada en la tarjeta NFC.");
  } else {
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
      delay(100);
      httpCode = https.GET();
      delay(100);
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
