#ifndef _CONTROLER_EVENTS_H
#define _CONTROLER_EVENTS_H

#include "../../KALibs/KALibs.hpp"

#include "memory.hpp"
#include "macros.hpp"

void onStart(unsigned uTime);

void onStop(bool bTotal);

void onControl(bool bControl);

void onSwitch(unsigned char uPin, bool bState);

void onClean(void);

void onRefresh(unsigned char& uDevices, const unsigned* puSensors);

void onEvents(unsigned char uParams, unsigned char uPins, unsigned uValue);

void onDevices(unsigned char uDefault);

#endif // _CONTROLER_EVENTS_H
