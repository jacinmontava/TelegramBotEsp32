/*
  Created by Jacin Montava
  02/10/2018
*/
/* LIBRARIES */
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <ssl_client.h>
#include <UniversalTelegramBot.h>

/* VARIABLES */
#define TELEGRAM_BUTTON 25 // buttonPin refers to ESP32 GPIO25 - D25
#define IFTT_BUTTON 35

const char* ssid     = "XXX";
const char* password = "XXX";
String ipAddress = "";
WiFiServer server(80);

/* BUTTONS STATES */
volatile bool telegramButtonPressedFlag = false;

/* TELEGRAM CONFIG */
#define BOT_TOKEN "XXX"  // your Bot Token (Get from Botfather)
#define CHAT_ID "XXX" // Chat ID of where you want the message to go (You can use MyIdBot to get the chat ID)

// SSL client needed for both libraries
WiFiClientSecure client;

UniversalTelegramBot bot(BOT_TOKEN, client);

/* FUNCTIONS */
void initHardware() {
  Serial.begin(115200);
  pinMode(TELEGRAM_BUTTON, INPUT);
  pinMode(IFTT_BUTTON, INPUT);

  // NOTE:
  // It is important to use interupts when making network calls in your sketch
  // if you just checked the status of te button in the loop you might
  // miss the button press.
  attachInterrupt(TELEGRAM_BUTTON, telegramButtonPressed, RISING);
}

void setupWiFi() {
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");

  IPAddress ip = WiFi.localIP();
  Serial.println(ip);

  ipAddress = ip.toString();

  server.begin();
}

void telegramButtonPressed() {
  Serial.println("telegramButtonPressed");
  int button = digitalRead(TELEGRAM_BUTTON);
  if (button == HIGH) {
    telegramButtonPressedFlag = true;
  }
  return;
}

void sendTelegramMessage() {
  String message = "SSID:  ";
  message.concat(ssid);
  message.concat("\n");
  message.concat("IP: ");
  message.concat(ipAddress);
  message.concat("\n");
  if (bot.sendMessage(CHAT_ID, message, "Markdown")) {
    Serial.println("TELEGRAM Successfully sent");
  }
  telegramButtonPressedFlag = false;
  Serial.println(message);
}


/* SETUP - VOID */
void setup() {
  initHardware();
  delay(10);
  setupWiFi();
}

void loop() {
  if (telegramButtonPressedFlag) {
    sendTelegramMessage();
  }
}
