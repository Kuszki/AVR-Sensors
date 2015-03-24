#include "memory.h"

extern Settings	SETTINGS;
extern Event*		EVENTS;

extern Timer		TIMER;
extern Diode		LED;
extern Flash		FLASH;

unsigned char LoadSettings(void)
{
	FLASH.SetAdress(0);

	FLASH.Read(&SETTINGS, sizeof(Settings));

	for (register unsigned char i = 0; i < DEVICES_COUNT; i++)
	{
		pinMode(i + 2, OUTPUT);
		digitalWrite(i + 2, SETTINGS.Devices & (1 << i));
	}

	ReloadEvents();

	TIMER.SetPrefs(Timer::P1024, SETTINGS.Refresh);

	if (TIMER.Start()) LED.On();

	return SETTINGS.Devices;
}

void SaveSettings(void)
{
	FLASH.SetAdress(0);

	FLASH.Write(&SETTINGS, sizeof(Settings));
}

void AddEvent(unsigned char uParams, unsigned char uPins, unsigned uValue)
{
	Event EVENT = { uParams, uPins, uValue };

	FLASH.Write(&EVENT, sizeof(Event));

	SETTINGS.Events++;
}

void ReloadEvents(void)
{
	if (!SETTINGS.Events) return;

	if (EVENTS) delete [] EVENTS;

	EVENTS = new Event[SETTINGS.Events];

	for (register unsigned char i = 0; i < SETTINGS.Events; i++)
		FLASH.Read(EVENTS + i, sizeof(Event));
}
