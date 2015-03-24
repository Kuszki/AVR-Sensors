#ifndef _CONTROLER_EVENTS_H
#define _CONTROLER_EVENTS_H

#include <Diode.h>
#include <Comport.h>
#include <Flash.h>
#include <Timer.h>

#include "memory.h"
#include "macros.h"

void onStart(unsigned uTime);

void onStop(bool bTotal);

void onControl(bool bControl);

void onSwitch(unsigned char uPin, bool bState);

void onClean(void);

void onRefresh(unsigned char& uDevices, const unsigned* puSensors);

void onEvents(unsigned char uParams, unsigned char uPins, unsigned uValue);

void onDevices(unsigned char uDefault);

#endif // _CONTROLER_EVENTS_H
