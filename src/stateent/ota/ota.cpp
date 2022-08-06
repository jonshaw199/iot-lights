
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#include "ota.h"
#include "state/stateManager.h"

void OTA::setup()
{
  Serial.println("OTA setting up");

  // JS - Issues with Heltec Wifi Kit 32; wouldnt connect
  // https://github.com/espressif/arduino-esp32/issues/1212
  // DO NOT TOUCH
  //  This is here to force the ESP32 to reset the WiFi and initialise correctly.
  // Serial.print("WIFI status = ");
  // Serial.println(WiFi.getMode());
  // WiFi.disconnect(true);
  // delay(1000);
  // WiFi.mode(WIFI_STA);
  // delay(1000);
  // Serial.print("WIFI status = ");
  // Serial.println(WiFi.getMode());
  // End silly stuff !!!

  WiFi.disconnect(true);
  delay(1000);
  WiFi.mode(WIFI_STA);
  Serial.println("MAC: " + WiFi.macAddress());
  delay(1000);

#if JS_IP_A
  // Set your Static IP address
  IPAddress local_IP(JS_IP_A, JS_IP_B, JS_IP_C, JS_IP_D);
  // Set your Gateway IP address
  IPAddress gateway(GATEWAY_A, GATEWAY_B, GATEWAY_C, GATEWAY_D);
  IPAddress subnet(SUBNET_A, SUBNET_B, SUBNET_C, SUBNET_D);
  // Configures static IP address
  if (!WiFi.config(local_IP, gateway, subnet))
  {
    Serial.println("Static IP Failed to configure");
  }
  else
  {
    Serial.println("Static IP config success");
  }
#endif

  // Connect to Wi-Fi network with SSID and password
  WiFi.begin(JSSSID, JSPASS);
  while (WiFi.waitForConnectResult() != WL_CONNECTED)
  {
    // Serial.println("Connection Failed! Rebooting...");
    // delay(5000);
    // ESP.restart();
    Serial.println("Connection failed! Retrying...");
    WiFi.begin(JSSSID, JSPASS);
  }

  StateManager::initWebSerial();

  // Port defaults to 3232
  // ArduinoOTA.setPort(3232);

  // Hostname defaults to esp3232-[MAC]
  // ArduinoOTA.setHostname("esp32-1");

  // No authentication by default
  // ArduinoOTA.setPassword("admin");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA
      .onStart([]()
               {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
      type = "sketch";
    else // U_SPIFFS
      type = "filesystem";

    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    Serial.println("Start updating " + type); })
      .onEnd([]()
             { Serial.println("\nEnd"); 
             StateManager::setRequestedState(STATE_RESTART); })
      .onProgress([](unsigned int progress, unsigned int total)
                  { Serial.printf("Progress: %u%%\r", (progress / (total / 100))); })
      .onError([](ota_error_t error)
               {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed"); });

  ArduinoOTA.begin();

  Serial.println("OTA Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void OTA::loop()
{
  ArduinoOTA.handle();
}

bool OTA::preStateChange(JSState s)
{
  StateManager::deinitWebSerial();
  return true;
}