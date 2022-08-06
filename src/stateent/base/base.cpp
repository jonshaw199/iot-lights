
#include "base.h"

Base::Base()
{
  // Do nothing by default; some children will need to setup(), etc.
}

void Base::setup()
{
}

void Base::loop()
{
}

bool Base::preStateChange(JSState s)
{
  return true;
}