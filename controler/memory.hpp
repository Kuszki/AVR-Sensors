#ifndef _CONTROLER_MEMORY_H
#define _CONTROLER_MEMORY_H

#include "../../KALibs/KALibs.hpp"

#include "macros.hpp"

typedef struct
{
	unsigned char Control;
	unsigned char Events;
	unsigned char Devices;

	unsigned Refresh;
} Settings;

typedef struct
{
	unsigned char Params;
	unsigned char Pins;

	unsigned Value;
} Event;

unsigned char LoadSettings(void);
void SaveSettings(void);

void AddEvent(unsigned char uParams, unsigned char uPins, unsigned uValue);

void ReloadEvents(void);

#endif // _CONTROLER_MEMORY_H
