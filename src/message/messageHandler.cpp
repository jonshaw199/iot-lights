
#include "messageHandler.h"
#include "util/wifi/wifiUtil.h"

MessageHandler::MessageHandler()
{
  WifiUtil::assignAPMac(macAP);
  WifiUtil::assignSTAMac(macSTA);
}

MessageHandler &MessageHandler::getInstance()
{
  static MessageHandler instance; // Guaranteed to be destroyed.
                                  // Instantiated on first use.
  return instance;
}

void MessageHandler::onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status)
{
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print("Last Packet Sent to: ");
  Serial.println(macStr);
  Serial.print("Last Packet Send Status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void MessageHandler::onDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len)
{
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  Serial.print("Last Packet Recv from: ");
  Serial.println(macStr);
  js_message msg;
  memcpy(&msg, incomingData, sizeof(msg));
  getInstance().inbox.push(msg);
}

void MessageHandler::init()
{
  initEspNow();
}

void MessageHandler::deinit()
{
  deinitEspNow();
}

void MessageHandler::initEspNow()
{
  WifiUtil::prepareWifi();
  if (esp_now_init() == ESP_OK)
  {
    Serial.println("ESP-NOW Init Success");
    esp_now_register_recv_cb(onDataRecv);
    esp_now_register_send_cb(onDataSent);
  }
  else
  {
    Serial.println("ESP-NOW Init Failed");
    // delay(3000);
    // ESP.restart();
  }
}

void MessageHandler::deinitEspNow()
{
  esp_now_deinit();
}

void MessageHandler::scanForPeers()
{
  int8_t networkCnt = WiFi.scanNetworks();
  if (networkCnt)
  {
    for (int i = 0; i < networkCnt; ++i)
    {
      String SSID = WiFi.SSID(i);
      int32_t RSSI = WiFi.RSSI(i);
      String BSSIDStr = WiFi.BSSIDstr(i);
      if (PRINT_WIFI_SCAN_RESULTS)
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
      // delay(10);
      // Check if the current network is one of our slaves
      int deviceID;
      if (sscanf(SSID.c_str(), "JS%x", &deviceID))
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
        // Get BSSID (MAC Address) of the Slave
        int mac[6];
        if (6 == sscanf(BSSIDStr.c_str(), "%x:%x:%x:%x:%x:%x", &mac[0], &mac[1], &mac[2], &mac[3], &mac[4], &mac[5]))
        {
          esp_now_peer_info_t info;
          for (int j = 0; j < 6; ++j)
          {
            info.peer_addr[j] = (uint8_t)mac[j];
          }
          info.channel = ESPNOW_CHANNEL;
          info.encrypt = 0; // no encryption
          getInstance().peerInfoMap[deviceID].espnowPeerInfo = info;
        }
      }
    }
  }
  else
  {
    Serial.println("No peers found");
  }

  // clean up ram
  WiFi.scanDelete();
}

void MessageHandler::connectToPeers()
{
  // Check if each slave is already connected to master
  // If not, then try to connect
  if (getInstance().peerInfoMap.size())
  {
    for (std::map<int, js_peer_info>::iterator it = getInstance().peerInfoMap.begin(); it != getInstance().peerInfoMap.end(); it++)
    {
      // Check if the peer exists
      if (!esp_now_is_peer_exist(it->second.espnowPeerInfo.peer_addr))
      {
        Serial.print("Peer ID ");
        Serial.print(it->first);
        Serial.println(" already paired");
      }
      else
      {
        // Slave not connected; attempt to connect
        esp_err_t connectStatus = esp_now_add_peer(&it->second.espnowPeerInfo);
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
          Serial.println("Unknown connection error");
        }

        delay(DELAY_MASTER_SLAVE_CONNECT);
      }
    }
  }
  else
  {
    // No slaves found in scan
    Serial.println("No slaves found to connect");
  }
}

void MessageHandler::sendMsg(JSMessage msg)
{
  // Also checking JSMessage recipients here; if empty then send to all, otherwise just send to the IDs in the set
  for (std::map<int, js_peer_info>::iterator it = getInstance().peerInfoMap.begin(); it != getInstance().peerInfoMap.end() && (!msg.getRecipients().size() || msg.getRecipients().find(it->first) != msg.getRecipients().end()); it++)
  {
    esp_err_t result = esp_now_send(it->second.espnowPeerInfo.peer_addr, (uint8_t *)&msg, sizeof(msg));
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

    delay(DELAY_MASTER_SLAVE_SEND);
  }
}

std::queue<JSMessage> &MessageHandler::getInbox()
{
  return getInstance().inbox;
}

void MessageHandler::sendHandshakeRequest(int id)
{
  Serial.println("Sending handshake request to ID " + id);
  JSMessage msg = JSMessage();
  msg.setRecipients({id});
  msg.setType(HANDSHAKE_REQUEST);
  msg.setSenderAPMac(getInstance().macAP);
  msg.setSenderID(JS_ID);
  memcpy(&getInstance().peerInfoMap[id].lastMsg, &msg, sizeof(msg));
  sendMsg(msg);
}

void MessageHandler::receiveHandshakeRequest(JSMessage m)
{
}

void MessageHandler::sendHandshakeResponse(int id)
{
}

void MessageHandler::receiveHandshakeResponse(JSMessage m)
{
}