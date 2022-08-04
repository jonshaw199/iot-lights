#include <Arduino.h>
#include <WiFi.h>

#include "master.h"
#include "state/stateManager.h"
#include "led/led.h"

void Master::setup()
{
  Serial.println("Master setting up");

  initESPNow();
}

void Master::loop()
{
  Serial.println("Master looping");

  if (curSlaveCnt < SLAVE_CNT)
  {
    scanForSlaves();
    connectToSlaves();
  }

  if (curSlaveCnt)
  {
    js_message msg;
    msg.s = "Testing...";
    msg.state = StateManager::getCurState();
    msg.color = LED::getRandColor();
    msg.recipients = {};
    sendData(msg);
  }

  delay(MASTER_LOOP_DELAY);
}

void Master::onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status)
{
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print("Last Packet Sent to: ");
  Serial.println(macStr);
  Serial.print("Last Packet Send Status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void Master::initESPNow()
{
  WiFi.disconnect(true);
  delay(1000);
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  Serial.println("MAC: " + WiFi.macAddress());

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

  esp_now_register_send_cb(Master::onDataSent);
}

void Master::scanForSlaves()
{
  int8_t networkCnt = WiFi.scanNetworks();
  // Reset slaves
  memset(slaves, 0, sizeof(slaves));
  curSlaveCnt = 0;
  if (networkCnt)
  {
    Serial.print("Found ");
    Serial.print(networkCnt);
    Serial.println(" networks");
    for (int i = 0; i < networkCnt; ++i)
    {
      String SSID = WiFi.SSID(i);
      int32_t RSSI = WiFi.RSSI(i);
      String BSSIDStr = WiFi.BSSIDstr(i);
      if (PRINT_SLAVE_SCAN_RESULTS)
      {
        // Print SSID and RSSI for each netowrk
        Serial.print(i + 1);
        Serial.print(": ");
        Serial.print(SSID);
        Serial.print(" [");
        Serial.print(BSSIDStr);
        Serial.print("]");
        Serial.print(" (");
        Serial.print(RSSI);
        Serial.print(")");
        Serial.println("");
      }
      delay(10);
      // Check if the current network is one of our slaves
      if (SSID.indexOf("Slave") == 0)
      {
        Serial.print(i + 1);
        Serial.print(": ");
        Serial.print(SSID);
        Serial.print(" [");
        Serial.print(BSSIDStr);
        Serial.print("]");
        Serial.print(" (");
        Serial.print(RSSI);
        Serial.print(")");
        Serial.println("");
        // Get BSSID => Mac Address of the Slave
        int mac[6];
        if (6 == sscanf(BSSIDStr.c_str(), "%x:%x:%x:%x:%x:%x", &mac[0], &mac[1], &mac[2], &mac[3], &mac[4], &mac[5]))
        {
          for (int j = 0; j < 6; ++j)
          {
            slaves[curSlaveCnt].peer_addr[j] = (uint8_t)mac[j];
          }
        }
        slaves[curSlaveCnt].channel = ESPNOW_CHANNEL;
        slaves[curSlaveCnt].encrypt = 0; // no encryption
        curSlaveCnt++;
      }
    }

    if (curSlaveCnt)
    {
      Serial.print(curSlaveCnt);
      Serial.println(" Slave(s) found, processing..");
    }
    else
    {
      Serial.println("No Slave Found, trying again.");
    }
  }
  else
  {
    Serial.println("No networks found");
  }

  // clean up ram
  WiFi.scanDelete();
}

void Master::connectToSlaves()
{
  // Check if each slave is already connected to master
  // If not, then try to connect
  if (curSlaveCnt)
  {
    for (int i = 0; i < curSlaveCnt; i++)
    {
      Serial.print("Processing: ");
      for (int j = 0; j < 6; ++j)
      {
        Serial.print((uint8_t)slaves[i].peer_addr[j], HEX);
        if (j != 5)
        {
          Serial.print(":");
        }
      }
      Serial.print(" Status: ");
      // Check if the peer exists
      if (esp_now_is_peer_exist(slaves[i].peer_addr))
      {
        Serial.println("Already Paired");
      }
      else
      {
        // Slave not connected; attempt to connect
        esp_err_t connectStatus = esp_now_add_peer(&slaves[i]);
        if (connectStatus == ESP_OK)
        {
          Serial.println("Pair success");
        }
        else if (connectStatus == ESP_ERR_ESPNOW_ARG)
        {
          Serial.println("Add Peer - Invalid Argument");
        }
        else if (connectStatus == ESP_ERR_ESPNOW_FULL)
        {
          Serial.println("Peer list full");
        }
        else if (connectStatus == ESP_ERR_ESPNOW_NO_MEM)
        {
          Serial.println("Out of memory");
        }
        else if (connectStatus == ESP_ERR_ESPNOW_EXIST)
        {
          Serial.println("Peer Exists");
        }
        else
        {
          Serial.println("Not sure what happened");
        }

        delay(MASTER_SLAVE_CONNECT_DELAY);
      }
    }
  }
  else
  {
    // No slaves found in scan
    Serial.print("No slaves found to connect");
  }
}

void Master::sendData(js_message data)
{
  for (int i = 0; i < curSlaveCnt; i++)
  {
    const uint8_t *peerAddr = slaves[i].peer_addr;
    // Print only for the first slave
    if (!i)
    {
      Serial.print("Sending data...");
    }
    esp_err_t result = esp_now_send(peerAddr, (uint8_t *)&data, sizeof(data));
    Serial.print("Send Status: ");
    if (result == ESP_OK)
    {
      Serial.println("Success");
    }
    else if (result == ESP_ERR_ESPNOW_NOT_INIT)
    {
      // How did we get so far!!
      Serial.println("ESPNOW not Init.");
    }
    else if (result == ESP_ERR_ESPNOW_ARG)
    {
      Serial.println("Invalid Argument");
    }
    else if (result == ESP_ERR_ESPNOW_INTERNAL)
    {
      Serial.println("Internal Error");
    }
    else if (result == ESP_ERR_ESPNOW_NO_MEM)
    {
      Serial.println("ESP_ERR_ESPNOW_NO_MEM");
    }
    else if (result == ESP_ERR_ESPNOW_NOT_FOUND)
    {
      Serial.println("Peer not found.");
    }
    else
    {
      Serial.println("Not sure what happened");
    }

    delay(MASTER_SLAVE_SEND_DELAY);
  }
}

bool Master::preStateChange(JSState s)
{
  if (StateManager::getCurState() == STATE_RUN && s == STATE_OTA)
  {
    // Inform the slaves so they also switch state
    Serial.println("Informing the slaves of OTA");
    js_message msg;
    msg.state = STATE_OTA;
    msg.recipients = {};
    sendData(msg);
    delay(3000);
  }
  return true;
}