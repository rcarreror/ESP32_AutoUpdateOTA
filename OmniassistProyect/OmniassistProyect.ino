//GENERAL
#include <Wire.h>
#define SERIAL_DEBUG_BAUD   115200

//WIFI
#include <WiFi.h>
#define WIFI_SSID           "WIFImovil"
#define WIFI_PASSWORD       "helloworld458"
int status = WL_IDLE_STATUS;                                             //inicialización del estado del WIFI
#include <WiFiClientSecure.h>
WiFiClientSecure secured_client;

//THINGSBOARD
#include <ThingsBoard.h>
#define TOKEN "BCjVM97zxPlb0BFlfcbR"                             // Access token obtenido del dispositivo creado
#define THINGSBOARD_SERVER  "demo.thingsboard.io"     // Utilizaremos el servidor gratuito de ThingsBoard
WiFiClient espClient;                                                             // Inicialiación del cliente de ThingsBoard
ThingsBoardSized<128> tb(espClient);                                  // Inicializar instancia de ThingsBoard redimensionando el buffer (para poder mandar mensajes más largos)

//ACELERÓMETRO
#include <Adafruit_MMA8451.h>
#include <Adafruit_Sensor.h>
Adafruit_MMA8451 mma = Adafruit_MMA8451();
int buzzer = 15;


//NFC
#include <PN532.h>
#include <NfcAdapter.h>
//para interfaz SPI (elegimos esta porque el PN532 no puede compartir bus I2C en ESP32)
#include <SPI.h>
#include <PN532_SPI.h>
PN532_SPI pn532spi(SPI, 5);  //el segundo parámetro es el pin al que está conectado SS
NfcAdapter nfc = NfcAdapter(pn532spi);
//para interfaz I2C
/*
  #include <PN532_I2C.h>
  PN532_I2C pn532_i2c(Wire);
  NfcAdapter nfc = NfcAdapter(pn532_i2c);
*/

//FECHA Y HORA
#include <ESP32Time.h>
ESP32Time rtc;
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 3600;
String horamin = "00:00";

//DISPLAY
#include <Adafruit_SSD1327.h>
#include <Fonts/FreeSerifBold9pt7b.h>
#include <Fonts/FreeSerifBold18pt7b.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#define OLED_RESET -1
Adafruit_SSD1327 display(128, 128, &Wire, OLED_RESET, 800000);
int pastilla1 = 0; //QUITAR. numero de pastilla que se ha tomado
int pastilla2 = 0; //QUITAR. numero de pastilla que se ha tomado
int pastilla3 = 0; //QUITAR. numero de pastilla que se ha tomado

//TELEGRAM BOT
#include <UniversalTelegramBot.h>
#define BOT_TOKEN "5012170312:AAFMpkM-q3BBwbcQ4DCdwlw9zUT-GnfFxhg" //sacado de BotFather
UniversalTelegramBot bot(BOT_TOKEN, secured_client);

//AUTO UPDATE OTA
#include <HTTPClient.h>
#include <HTTPUpdate.h>
#include "cert.h"
#define URL_fw_Version   "https://raw.githubusercontent.com/rcarreror/ESP32_AutoUpdateOTA/main/OmniassistProyect/bin_version.txt"
#define URL_fw_Bin           "https://raw.githubusercontent.com/rcarreror/ESP32_AutoUpdateOTA/main/OmniassistProyect/fw.bin"
String FirmwareVer = {
  "1.1"
};



//JSON
#include <EEPROM.h>//https://github.com/espressif/arduino-esp32/tree/master/libraries/EEPROM
#include <ArduinoJson.h>
#include <StreamUtils.h>
StaticJsonDocument<1536> confJSON;
#define EEPROM_SIZE 1536

//PERIODOS TAREAS
const unsigned long periodNFC = 5000; //En milisegundos
const unsigned long periodFall = 200; //En milisegundos
const unsigned long periodDisplay = 1000; //En milisegundos
const unsigned long periodBot  = 1000;
const unsigned long periodOTA  = 60000;



void setup() {
  Serial.begin(SERIAL_DEBUG_BAUD);
  Serial.println();
  initDisplay();
  nfc.begin();
  initAcc();
  initWiFi();
  initTB();
  initTime();
  secured_client.setCACert(TELEGRAM_CERTIFICATE_ROOT);
  initConfig();
  display.fillRect(0, 0, 128, 128, SSD1327_BLACK); //Para no tener que limpiar toda la pantalla
}

void loop() {
  displayTask();
  if (WiFi.status() != WL_CONNECTED) { //comprueba si seguimos conectados a la red Wifi
    Serial.println("Se ha desconectado la red WIFI.");
    drawWifi(-100);
    display.display();
    initWiFi();
  }

  accTask();
  nfcReader();
  autoUpdateTask();
  //tb.loop();
}

//------------------------------------TAREA 1------------------------------------
//Comprueba si hay una tarjeta NFC, en el caso de que la haya, escribe
//en ella la fecha y hora actual y la envía como telemetría a ThingsBoard
void nfcReader() {
  static unsigned long previousMillis = 0;
  if ((millis() - previousMillis) > periodNFC) {
    if (nfc.tagPresent()) {
      //A partir de aquí no escribir nada en el serial porque estropea el display
      char msg[] = " Ultima toma de pastilla: 18:34";
      sprintf(msg, "Ultima toma de pastilla: %s", horamin);
      NfcTag tag = nfc.read();
      String idTag = tag.getUidString();
      escribirFecha(msg);
      if (idTag.equals("4C 30 FE 38")) {
        tb.sendTelemetryString("pastilla1",  msg);
        pastilla1++;
      }
      else if (idTag.equals("EA C2 5D DB")) {
        tb.sendTelemetryString("pastilla2",  msg);
        pastilla2++;
      }
      else if (idTag.equals("73 8E 05 07")) {
        tb.sendTelemetryString("pastilla3",  msg);
        pastilla3++;
      }
      else startConfig();
      Serial.println("Información enviada al ThingsBoard");

    }
    previousMillis += periodNFC;
  }
}

//------------------------------------TAREA 2------------------------------------
void accTask() {
  static unsigned long previousMillis = 0;

  if ((millis() - previousMillis) > periodFall) {
    if (! mma.begin()) {
      return;
    } else {
      mma.read();
      //Get a new sensor event
      sensors_event_t event;
      mma.getEvent(&event);

      long int xa = event.acceleration.x;
      long int ya = event.acceleration.y;
      long int za = event.acceleration.z;
      float at = sqrt(xa * xa + ya * ya + za * za);

      if (at > 20) {
        digitalWrite(buzzer, HIGH);
        Serial.println(at);
      } else digitalWrite(buzzer, LOW);
    }

    previousMillis += periodFall;
  }
}

//------------------------------------TAREA 3------------------------------------
void displayTask() {
  static unsigned long previousMillis = 0;

  if ((millis() - previousMillis) > periodDisplay) {
    drawBitmap();
    display.display();
    previousMillis += periodDisplay;
  }
}

//------------------------------------TAREA 4------------------------------------
void autoUpdateTask() {
  static unsigned long previousMillis = 0;
  if ((millis() - previousMillis) > periodOTA) {
    Serial.print("Active firmware version: ");
    Serial.println(FirmwareVer);
    if (FirmwareVersionCheck()) {
      firmwareUpdate();
    }
    previousMillis += periodOTA;
  }
}