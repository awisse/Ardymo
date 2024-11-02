// Leave debug code here for now. Takes no space on Arduboy
#include "debug.h"
#include "platform.h"
#ifdef DEBUG_
void printSensors(SensorValues* sensors) {
  Platform::DebugPrintln("=====Sensors=====");
  Platform::DebugPrint("left, right, front, rear: (");
  Platform::DebugPrint(sensors->distances.left);
  Platform::DebugPrint(", ");
  Platform::DebugPrint(sensors->distances.right);
  Platform::DebugPrint(", ");
  Platform::DebugPrint(sensors->distances.front);
  Platform::DebugPrint(", ");
  Platform::DebugPrint(sensors->distances.rear);
  Platform::DebugPrintln(")");
  Platform::DebugPrint("tgt_dist, speed, collision, on_target: ( ");
  Platform::DebugPrint(sensors->tgt_distance);
  Platform::DebugPrint(", ");
  Platform::DebugPrint(sensors->speed);
  Platform::DebugPrint(", ");
  Platform::DebugPrint(sensors->collision);
  Platform::DebugPrint(", ");
  Platform::DebugPrint(sensors->on_target ? "over" : "fine");
  Platform::DebugPrintln(")");
}
void printShared(SharedData* shared) {
  Platform::DebugPrintln("=====Shared=====");
  Platform::DebugPrint("left, right: (");
  Platform::DebugPrint(shared->dist_left);
  Platform::DebugPrint(", ");
  Platform::DebugPrint(shared->dist_right);
  Platform::DebugPrintln(")");
  Platform::DebugPrint("collision, on_target: ( ");
  Platform::DebugPrint(shared->collision);
  Platform::DebugPrint(", ");
  Platform::DebugPrint(shared->on_target ? "over" : "fine");
  Platform::DebugPrintln(")");
}
#endif // DEBUG_
// vim: tabstop=2:softtabstop=2:shiftwidth=2:expandtab:filetype=cpp
