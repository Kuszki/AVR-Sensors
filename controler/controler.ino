#include <Diode.h>
#include <Comport.h>
#include <Flash.h>
#include <Timer.h>

#include "macros.h"

Timer	TIMER(Timer::T2);
Comport	RS232(9600);
Diode	LED(13);
Flash	FLASH;

Diode*	DEVICES = nullptr;

void onStart(unsigned);
void onStop(void);

void onManual(unsigned char, bool);

void setup(void)
{
	for (unsigned char i = 2; i < 13; i++)
		pinMode(i, OUTPUT);

	RS232.Start();
}

void loop(void)
{
	static unsigned char RecvData[DATA_FRAME_SIZE];

	while (RS232.Recv(RecvData, DATA_FRAME_SIZE))
	switch (RecvData[0])
	{
		case SIGNAL_START:
			onStart(RecvData[1] + (RecvData[2] << 8));
		break;
		case SIGNAL_STOP:
			onStop();
		break;
		case SIGNAL_MANUAL:
			onManual(RecvData[1], RecvData[2]);
		break;
	}
}

void onStart(unsigned uTime)
{
	TIMER.SetPrefs(Timer::P1024, uTime);

	if (TIMER.Start()) LED.On();
}

void onStop(void)
{
	TIMER.Stop();
	LED.Off();
}

void onManual(unsigned char uPin, bool bState)
{
	digitalWrite(uPin, bState);
}

ISR(TIMER1_COMPA_vect)
{
	static unsigned SendData[SENSORS_COUNT];

	for (register unsigned char i = 0; i < SENSORS_COUNT; i++)
		SendData[i] = analogRead(i);

	RS232.Send(SendData, sizeof(unsigned) * SENSORS_COUNT);
}
