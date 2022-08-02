#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>

#include "slave.h"
#include "state/stateManager.h"

Slave::Slave()
{
}

void Slave::setup()
{
  // Set device in AP mode to begin with
  WiFi.mode(WIFI_AP);
  // configure device AP mode
  configDeviceAP();
  // This is the mac address of the Slave in AP Mode
  Serial.print("AP MAC: ");
  Serial.println(WiFi.softAPmacAddress());
  // Init ESPNow with a fallback logic
  initESPNow();
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info.
  esp_now_register_recv_cb(Slave::onDataRecv);

  led = LED();
  led.setup();
  Serial.println("Slave setup complete");
}

void Slave::loop()
{
  // Check msg queue for new messages
  if (StateManager::getMsgQueue().size())
  {
    js_message msg;
    // Just taking the most recent msg for now
    while (StateManager::getMsgQueue().size())
    {
      msg = StateManager::dequeMsg();
    }

    if (StateManager::getCurState() == msg.state)
    {
      switch (msg.state)
      {
      case STATE_RUN:
        led.fillColor(msg.color);
        break;
      default:
        Serial.print("State not found: ");
        Serial.println(msg.state);
      }
    }
    else
    {
      // State change requested by master
      StateManager::setRequestedState(msg.state);
    }
  }
}

void Slave::onDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len)
{
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  Serial.print("Last Packet Recv from: ");
  Serial.println(macStr);
  js_message msg;
  memcpy(&msg, incomingData, sizeof(msg));
  StateManager::queueMsg(msg);
}

// Init ESP Now with fallback
void Slave::initESPNow()
{
  WiFi.disconnect();
  delay(1000);
  if (esp_now_init() == ESP_OK)
  {
    Serial.println("ESPNow Init Success");
  }
  else
  {
    Serial.println("ESPNow Init Failed");
    // Retry InitESPNow, add a counte and then restart?
    // InitESPNow();
    // or Simply Restart
    ESP.restart();
  }
}

// config AP SSID
void Slave::configDeviceAP()
{
  String Prefix = "Slave:";
  String Mac = WiFi.macAddress();
  String SSID = Prefix + Mac;
  String Password = "123456789";
  bool result = WiFi.softAP(SSID.c_str(), Password.c_str(), ESPNOW_CHANNEL, 0);
  if (!result)
  {
    Serial.println("AP Config failed.");
  }
  else
  {
    Serial.println("AP Config Success. Broadcasting with AP: " + String(SSID));
  }
}

bool Slave::preStateChange(JSState s)
{
  return led.preStateChange(s);
}