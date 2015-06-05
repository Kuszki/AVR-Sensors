#include "events.hpp"

extern Settings		SETTINGS;
extern Event*		EVENTS;

extern KATimer		TIMER;
extern KAOutput		LED;

extern bool 		Connected;

void onStart(unsigned uTime)
{
	Connected = true;

	TIMER.SetPrefs(KATimer::P_1024, uTime);

	SETTINGS.Refresh = uTime;

	TIMER.Start();
	LED.On();

	SaveSettings();
}

void onStop(bool bTotal)
{
	Connected = false;

	if (bTotal || !SETTINGS.Control)
	{
		TIMER.Stop();
		LED.Off();

		SETTINGS.Refresh = 0;
		SETTINGS.Control = 0;
	}

	SaveSettings();
}

void onControl(bool bControl)
{
	SETTINGS.Control = bControl;

	SaveSettings();
}

void onSwitch(unsigned char uPin, bool bState)
{
	//KAOutput::SetState(uPin, bState);
}

void onRefresh(unsigned char& uDevices, const unsigned* puSensors)
{
	for (register unsigned char i = 0; i < SETTINGS.Events; i++)
	{
		Event& EVENT = EVENTS[i];

		if (EVENT.Params & 0x0F)
		{
			//if (puSensors[EVENT.Pins & 0x0F] > EVENT.Value) bitWrite(uDevices, EVENT.Pins >> 4, EVENT.Params & 0xF0);
		}
		else
		{
			//if (puSensors[EVENT.Pins & 0x0F] < EVENT.Value) bitWrite(uDevices, EVENT.Pins >> 4, EVENT.Params & 0xF0);
		}
	}
}

void onEvents(unsigned char uParams, unsigned char uPins, unsigned uValue)
{
	switch (uParams)
	{
		case 0x00:
			SETTINGS.Events = 0;
			SaveSettings();
		break;
		case 0xFF:
			SaveSettings();
			ReloadEvents();
		break;
		default:
			AddEvent(uParams, uPins, uValue);
	}
}

void onDevices(unsigned char uDefault)
{
	SETTINGS.Devices = uDefault;

	SaveSettings();
}
