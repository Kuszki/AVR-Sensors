#include "../../KALibs/KALibs.hpp"
#include "../../KLLibs/KLLibs.hpp"

#include "events.hpp"
#include "memory.hpp"
#include "macros.hpp"

Settings	SETTINGS	= {0, 0};
Event*		EVENTS		= 0;

KATimer		TIMER(KATimer::T_2);
KAUart		RS232(9600);
KAOutput	LED(PIN_13);
KAFlash		FLASH;

volatile unsigned char OldState;
volatile unsigned char NewState;

bool Connected;

int main(void)
{

	LoadSettings();

	OldState = NewState =  SETTINGS.Devices;

	RS232.Start();

	while (true)
	{

		static unsigned char RecvData[DATA_FRAME_SIZE];

		while (RS232.Recv(RecvData, DATA_FRAME_SIZE))
		switch (RecvData[0])
		{
			case SIGNAL_STOP:
				onStop(RecvData[1]);
			break;
			case SIGNAL_START:
				onStart((RecvData[1] << 8) + RecvData[2]);
			break;
			case SIGNAL_CONTROL:
				onControl(RecvData[1]);
			break;
			case SIGNAL_SWITCH:
				onSwitch(RecvData[1], RecvData[2]);
			break;
			case SIGNAL_EVENTS:
				onEvents(RecvData[1], RecvData[2], (RecvData[3] << 8) + RecvData[4]);
			break;
			case SIGNAL_DEVICES:
				onDevices(RecvData[1]);
			break;
		}

		if (SETTINGS.Control && (OldState != NewState))
		{
			//for (register unsigned char i = 0; i < DEVICES_COUNT; i++)
			//	KAOutput::SetState(i + 2, NewState & (1 << i));

			OldState = NewState;
		}

	}
}

ISR(TIMER1_COMPA_vect)
{
	static unsigned SensorData[SENSORS_COUNT];

	unsigned char State = 0;

	for (register unsigned char i = 0; i < SENSORS_COUNT; i++)
		SensorData[i] = KAConverter::GetValue(KAConverter::PORT(i));

	//if (SETTINGS.Control)
	//	onRefresh(State, SensorData);

	if (Connected)
	{
		RS232.Send(SensorData, sizeof(unsigned) * SENSORS_COUNT);
		RS232.Send(&State, sizeof(unsigned char));
	}

	NewState = State;
}
