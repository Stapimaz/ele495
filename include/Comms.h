/**
 * @file Comms.h
 * @brief Robotun C# (UI) uygulaması ile haberleşmesini sağlayan UDP/WiFi prototiplerini içerir.
 */
#ifndef COMMS_H
#define COMMS_H

#include "Config.h"

void initComms();
void sendTelemetryData();
void checkForIncomingCommands();

#endif // COMMS_H
