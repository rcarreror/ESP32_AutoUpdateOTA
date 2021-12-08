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
String horaminPrev = "01:01";
String horamin = "00:00";

//DISPLAY
#include <Adafruit_SSD1327.h>
#include <Fonts/FreeSerifBold9pt7b.h>
#include <Fonts/FreeSerifBold18pt7b.h>
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
#define URL_fw_Version   "https://raw.githubusercontent.com/rcarreror/ESP32_AutoUpdateOTA/main/OmnitrixProyect/bin_version.txt"
#define URL_fw_Bin           "https://raw.githubusercontent.com/rcarreror/ESP32_AutoUpdateOTA/main/OmnitrixProyect/fw.bin"
String FirmwareVer = {
  "1.0"
};



//JSON


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
  initWiFi();
  initTB();
  initAcc();
  initTime();
  secured_client.setCACert(TELEGRAM_CERTIFICATE_ROOT);
  display.fillRect(0, 0, 128, 128, SSD1327_BLACK); //Para no tener que limpiar toda la pantalla
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) { //comprueba si seguimos conectados a la red Wifi
    Serial.println("Se ha desconectado la red WIFI.");
    initWiFi();
  }

  if (!tb.connected()) {  //comprueba si seguimos conectados con ThingsBoard
    Serial.println("Se ha perdido la conexión con ThingsBoard");
    initTB();
  }

  telegramTask();
  seHaCaio();
  nfcReader();
  displayTask();
  autoUpdateTask();
  //tb.loop();
}

//------------------------------------TAREA 1------------------------------------
//Comprueba si hay una tarjeta NFC, en el caso de que la haya, escribe
//en ella la fecha y hora actual y la envía como telemetría a ThingsBoard
void nfcReader() {
  static unsigned long previousMillis = 0;
  if ((millis() - previousMillis) > periodNFC) {
    Serial.println("\nScan a NFC tag\n");
    if (nfc.tagPresent()) {
      //A partir de aquí no escribir nada en el serial porque estropea el display
      char msg[] = " Ultima toma de pastilla: 18:34";
      sprintf(msg, "Ultima toma de pastilla: %s", horamin);
      escribirFecha(msg);
      NfcTag tag = nfc.read();
      if (tag.hasNdefMessage()) {
        //TODO: Comprobar qué tarjeta es (y en consecuencia, qué pastilla), segun su tag con tag.getUidString() o tag.getUid()
        if (tag.getUidString().equals("4C 30 FE 38")) {
          tb.sendTelemetryString("pastilla1",  msg);
          pastilla1++;
        }
        if (tag.getUidString().equals("9C 03 5F 6D")) {
          tb.sendTelemetryString("pastilla2",  msg);
          pastilla2++;
        }
        if (tag.getUidString().equals("73 8E 05 07")) {
          tb.sendTelemetryString("pastilla3",  msg);
          pastilla3++;
        }
      } else Serial.println("La tarjeta NFC no tiene mensaje");
    }
    previousMillis += periodNFC;
  }
}

//------------------------------------TAREA 2------------------------------------
void seHaCaio() {
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

      if (at > 30) {
        Serial.println("----------------------------------SE HA CAIDO MR PAKO----------------------------------");
        Serial.println(at);
      }
    }
    previousMillis += periodFall;
  }
}

//------------------------------------TAREA 3------------------------------------
void displayTask() {
  static unsigned long previousMillis = 0;

  if ((millis() - previousMillis) > periodDisplay) {
    horaminPrev = horamin;
    horamin = rtc.getTime();
    if (horamin != horaminPrev) { //Actualiza cuando cambian la hora y al iniciar el programa
      display.setTextSize(1);
      display.setTextColor(SSD1327_WHITE);
      display.setFont(&FreeSerifBold18pt7b);
      display.setCursor(22, 30);
      horaminPrev = horamin;
      //display.clearDisplay();
      display.fillRect(0, 0, 128, 40, SSD1327_BLACK); //Para no tener que limpiar toda la pantalla
      display.print(horamin);
      //display.display();
    }
    drawBitmap();
    display.display();
    previousMillis += periodDisplay;
  }
}


//------------------------------------TAREA 4------------------------------------
void telegramTask() {
  static unsigned long previousMillis = 0;
  if ((millis() - previousMillis) > periodBot) {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while (numNewMessages)
    {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }

    previousMillis += periodBot;
  }
}


//------------------------------------TAREA 5------------------------------------
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
