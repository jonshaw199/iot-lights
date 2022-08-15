/*
  AF1 - An Arduino extension framework
  Copyright (c) 2022 Jon Shaw. All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 3 of the license, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <Arduino.h>

#include "purg.h"
#include "stateManager/stateManager.h"
#include "message/message.h"
#include "messageHandler/messageHandler.h"

Purg::Purg()
{
  purgMs = MS_PURG_DEFAULT;
  next = STATE_NONE;
}

Purg::Purg(int s)
{
  purgMs = MS_PURG_DEFAULT;
  next = s;
}

void Purg::setup()
{
  Base::setup();

#ifdef MASTER
  JSMessage msg;
  msg.setType(TYPE_CHANGE_STATE);
  msg.setState(next);
  msg.setMaxRetries(RETRIES_PURG);
  MessageHandler::pushOutbox(msg);
#endif
}

void Purg::loop()
{
  Base::loop();

  if (getElapsedMs() > purgMs)
  {
    Serial.println("Purgatory over");
    StateManager::setRequestedState(next);
  }
}

void Purg::setPurgMs(unsigned long ms)
{
  purgMs = ms;
}

void Purg::setNext(int s)
{
  next = s;
}
