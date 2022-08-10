#ifndef STATE_STATE_H_
#define STATE_STATE_H_

enum JSState
{
  STATE_NONE,
  STATE_INIT,
  STATE_RUN,
  STATE_PURG_OTA,
  STATE_OTA,
  STATE_IDLE,
  STATE_RESTART,
  STATE_HANDSHAKE
};

#endif // STATE_STATE_H