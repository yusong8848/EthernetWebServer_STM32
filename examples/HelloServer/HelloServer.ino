/****************************************************************************************************************************
 * HelloServer.ino - Dead simple web-server for Ethernet shields
 *
 * For STM32 with built-in Ethernet (Nucleo-144, DISCOVERY, etc)
 *
 * EthernetWebServer_STM32 is a library for the STM32 run built-in Ethernet WebServer
 *
 * Forked and modified from ESP8266 https://github.com/esp8266/Arduino/releases
 * Built by Khoi Hoang https://github.com/khoih-prog/ESP8266_AT_WebServer
 * Licensed under MIT license
 * Version: 1.0.2
 *
 * Original author:
 * @file       Esp8266WebServer.h
 * @author     Ivan Grokhotkov
 *
 * Version Modified By   Date      Comments
 * ------- -----------  ---------- -----------
 *  1.0.0   K Hoang      26/02/2020 Initial coding for STM32 with built-in Ethernet (Nucleo-144, DISCOVERY, etc) and ENC28J60
 *  1.0.1   K Hoang      28/02/2020 Add W5x00 Ethernet shields using Ethernet library
 *  1.0.2   K Hoang      05/03/2020 Remove dependency on functional-vlpp
 *****************************************************************************************************************************/
/*
 * Currently support 
 * 1) STM32 boards with built-in Ethernet (to use USE_BUILTIN_ETHERNET = true) such as :
 *    - Nucleo-144 (F429ZI, F767ZI)
 *    - Discovery (STM32F746G-DISCOVERY)
 *    - All STM32 Boards with Built-in Ethernet, See How To Use Built-in Ethernet at (https://github.com/khoih-prog/EthernetWebServer_STM32/issues/1)
 * 2) STM32 boards (with 64+K Flash) running EMC28J60 shields (to use USE_BUILTIN_ETHERNET = false)
 * 3) STM32 boards (with 32+K Flash) running W5x00 Ethernet shields
 * 
 */

#if defined(ESP8266) || defined(ESP32) || defined(AVR) || (ARDUINO_SAM_DUE)
#error This code is designed to run on STM32 platform, not AVR, SAM DUE, SAMD, ESP8266 nor ESP32! Please check your Tools->Board setting.
#endif

#define USE_BUILTIN_ETHERNET    true
//  If don't use USE_BUILTIN_ETHERNET, and USE_UIP_ETHERNET => use W5x00 with Ethernet library
#define USE_UIP_ETHERNET        false 

#if (USE_BUILTIN_ETHERNET)
  #define ETHERNET_NAME     "Built-in LAN8742A Ethernet"
#elif (USE_UIP_ETHERNET)
  #define ETHERNET_NAME     "ENC28J60 Ethernet Shield"
#else
  #define ETHERNET_NAME     "W5x00 Ethernet Shield"
#endif

#if defined(STM32F0)
  #warning STM32F0 board selected
  #define DEVICE_NAME  "STM32F0"
#elif defined(STM32F1)
  #warning STM32F1 board selected
  #define DEVICE_NAME  "STM32F1"
#elif defined(STM32F2)
  #warning STM32F2 board selected
  #define DEVICE_NAME  "STM32F2"
#elif defined(STM32F3)
  #warning STM32F3 board selected
  #define DEVICE_NAME  "STM32F3"
#elif defined(STM32F4)
  #warning STM32F4 board selected
  #define DEVICE_NAME  "STM32F4"
#elif defined(STM32F7)
  #warning STM32F7 board selected
  #define DEVICE_NAME  "STM32F7"  
#else
  #warning STM32 unknown board selected
  #define DEVICE_NAME  "STM32 Unknown"  
#endif

#include <EthernetWebServer_STM32.h>
  
// Enter a MAC address and IP address for your controller below.

byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};

// Select the IP address according to your local network
IPAddress ip(192, 168, 2, 200);

EthernetWebServer server(80);

const int led = 13;

void handleRoot() 
{
  server.send(200, "text/plain", "Hello from EthernetWebServer");
}

void handleNotFound()
{
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++)
  {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
}

void setup(void)
{
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  delay(1000);
  Serial.println("\nStart HelloServer on " + String(DEVICE_NAME) + " board, running " + String(ETHERNET_NAME));

  // start the ethernet connection and the server:
  Ethernet.begin(mac, ip);

  server.on("/", handleRoot);

  server.on("/inline", [](){
    server.send(200, "text/plain", "This works as well");
  });

  server.onNotFound(handleNotFound);

  server.begin();

  Serial.print(F("HTTP EthernetWebServer is @ IP : "));
  Serial.println(Ethernet.localIP());
}

void loop(void)
{
  server.handleClient();
}
