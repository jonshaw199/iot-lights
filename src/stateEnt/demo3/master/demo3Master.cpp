
#include "demo3Master.h"
#include "stateEnt/demo3/demo3.h"

demo3Master::demo3Master()
{
  intervalEvents.push_back(IntervalEvent(MS_DEMO3_LOOP, demo3));
}

bool demo3Master::demo3(IECBArg a)
{
  return true;
}
