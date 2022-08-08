
#include "messageHandler.h"
#include "util/wifi/wifiUtil.h"

MessageHandler::MessageHandler()
{
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
  if (status == ESP_NOW_SEND_SUCCESS)
  {
    Serial.println("Delivery Success");
  }
  else
  {
    Serial.println("Delivery Fail");
    int peerDeviceID = getInstance().macToIDMap[WifiUtil::macToString(mac_addr)];
    // Check if there are more retries remaining and retry if so
    if (getInstance().peerInfoMap[peerDeviceID].lastMsg.getSendCnt() + 1 < getInstance().peerInfoMap[peerDeviceID].lastMsg.getMaxRetries())
    {
      Serial.println("Retrying send to device ID " + String(peerDeviceID));
      JSMessage msg = getInstance().peerInfoMap[peerDeviceID].lastMsg;
      msg.incrementSendCnt();
      msg.setRecipients({peerDeviceID}); // Only resending to 1 device!
      // sendMsg(msg);
      getInstance().outbox.push(msg);
    }
  }
}

bool MessageHandler::validateMsg(JSMessage m)
{
  return true;
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
  JSMessage msgWrapper = msg;
  if (validateMsg(msgWrapper))
  {
    getInstance().inbox.push(msgWrapper);
  }
}

void MessageHandler::init()
{
  WifiUtil::setAPMode();
  WiFi.softAPmacAddress(getInstance().macAP);
  Serial.print("MAC AP: ");
  WifiUtil::printMac(getInstance().macAP);
  WifiUtil::setSTAMode();
  WiFi.macAddress(getInstance().macSTA);
  Serial.print("MAC STA: ");
  WifiUtil::printMac(getInstance().macSTA);
}

void MessageHandler::initEspNow()
{
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

void MessageHandler::deinit()
{
  esp_now_deinit();
}

void MessageHandler::scanForPeers(bool overwriteExisting)
{
  Serial.println("Scanning for peers");
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
      // Check if the current network is one of our slaves
      int deviceID;
      if (sscanf(SSID.c_str(), "JS%x", &deviceID))
      {
        // Check the overwrite argument and only overwrite existing entries if true
        if (!getInstance().peerInfoMap.count(deviceID) || overwriteExisting)
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
            memset(&info, 0, sizeof(info));
            for (int j = 0; j < 6; ++j)
            {
              info.peer_addr[j] = (uint8_t)mac[j];
            }
            info.channel = ESPNOW_CHANNEL;
            info.encrypt = 0; // no encryption
            info.ifidx = WIFI_IF_AP;
            getInstance().peerInfoMap[deviceID].espnowPeerInfo = info;
            getInstance().peerInfoMap[deviceID].handshakeResponse = false;
            // getInstance().peerInfoMap[deviceID].lastMsg = JSMessage();
            getInstance().macToIDMap[WifiUtil::macToString(info.peer_addr)] = deviceID;
            Serial.println("Saved peer info for device ID " + String(deviceID));
          }
        }
        else
        {
          Serial.println("Peer info already collected for device ID " + String(deviceID));
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
      if (esp_now_is_peer_exist(it->second.espnowPeerInfo.peer_addr))
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
    Serial.print("Sending message to device ID " + String() + " (MAC address " + WifiUtil::macToString(it->second.espnowPeerInfo.peer_addr) + ")");
    esp_err_t result = esp_now_send(it->second.espnowPeerInfo.peer_addr, (uint8_t *)&msg, sizeof(msg));
    Serial.print("Send Status: ");
    if (result == ESP_OK)
    {
      Serial.println("Success");

      // Update last msg sent for this peer
      memcpy(&getInstance().peerInfoMap[it->first].lastMsg, &msg, sizeof(msg));
      // Update the send count of that last msg
      getInstance().peerInfoMap[it->first].lastMsg.incrementSendCnt();
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
    else if (result == ESP_ERR_ESPNOW_IF)
    {
      Serial.println("Current wifi interface doesnt match that of peer");
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

std::queue<JSMessage> &MessageHandler::getOutbox()
{
  return getInstance().outbox;
}

void MessageHandler::sendHandshakeRequests(std::set<int> ids)
{
  Serial.println("Sending handshake requests");

  JSMessage msg = JSMessage();

  // Set struct
  msg.setType(TYPE_HANDSHAKE_REQUEST);
  msg.setSenderID(JS_ID);
  msg.setState(STATE_HANDSHAKE);
  msg.setSenderAPMac(getInstance().macAP);
  // Set wrapper
  msg.setRecipients(ids);

  // sendMsg(msg);
  getInstance().outbox.push(msg);

  for (std::set<int>::const_iterator it = ids.begin(); it != ids.end(); it++)
  {
    getInstance().peerInfoMap[*it].handshakeRequest = true;
  }
}

void MessageHandler::receiveHandshakeRequest(JSMessage m)
{
  Serial.println("Receiving handshake request from ID " + String(m.getSenderID()));
  Serial.print("Mac: ");
  WifiUtil::printMac(m.getSenderAPMac());
  js_peer_info i;
  memset(&i, 0, sizeof(i));
  memcpy(&i.espnowPeerInfo.peer_addr, m.getSenderAPMac(), 6);
  WifiUtil::printMac(i.espnowPeerInfo.peer_addr);
  i.espnowPeerInfo.channel = ESPNOW_CHANNEL;
  i.espnowPeerInfo.encrypt = 0; // No encryption
  // getInstance().peerInfoMap[m.getSenderID()] = i;
  i.espnowPeerInfo.ifidx = WIFI_IF_AP;
  memcpy(&getInstance().peerInfoMap[m.getSenderID()], &i, sizeof(i));
  connectToPeers();
}

void MessageHandler::sendHandshakeResponses(std::set<int> ids)
{
  Serial.println("Sending handshake responses");

  JSMessage msg = JSMessage();

  // Set struct
  msg.setType(TYPE_HANDSHAKE_RESPONSE);
  msg.setSenderID(JS_ID);
  msg.setState(STATE_HANDSHAKE);
  // Set wrapper
  msg.setRecipients(ids);

  // sendMsg(msg);
  getInstance().outbox.push(msg);
}

void MessageHandler::receiveHandshakeResponse(JSMessage m)
{
  Serial.println("Receiving handshake response from ID " + String(m.getSenderID()));
  getInstance().peerInfoMap[m.getSenderID()].handshakeResponse = true;
}

const std::map<int, js_peer_info> &MessageHandler::getPeerInfoMap()
{
  return getInstance().peerInfoMap;
}

// Shortcut for getting the most recent msg in the queue and popping along the way
JSMessage MessageHandler::getAndPopInbox()
{
  JSMessage m;
  while (MessageHandler::getInbox().size())
  {
    m = MessageHandler::getInbox().front();
    MessageHandler::getInbox().pop();
  }
  return m;
}

JSMessage MessageHandler::popAndFrontInbox()
{
  JSMessage m;
  if (MessageHandler::getInbox().size())
  {
    while (MessageHandler::getInbox().size() > 1)
    {
      MessageHandler::getInbox().pop();
    }
    m = MessageHandler::getInbox().front();
  }
  return m;
}
