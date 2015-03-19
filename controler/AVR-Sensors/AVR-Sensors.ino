#include <Diode.h>
#include <Comport.h>
#include <Flash.h>
#include <Timer.h>

#define DEVICES_COUNT 8
#define SENSORS_COUNT 6
#define DATA_FRAME_SIZE 5

#define CONTROL_MANUAL 0
#define CONTROL_REMOTE 2
#define CONTROL_AUTO 4

#define SIGNAL_STOP 0
#define SIGNAL_START 1
#define SIGNAL_CONTROL 2
#define SIGNAL_MANUAL 3

unsigned SendData[SENSORS_COUNT];

Timer TIMER(Timer::T2);
Comport RS232(9600);
Diode LED(13);
Flash FLASH;

Diode* DEVICES = nullptr;

void onStart(unsigned);
void onStop(void);

void onManual(unsigned char, bool);

void setup()
{    
     for (unsigned char i = 2; i < 13; i++) pinMode(i, OUTPUT);
     
     RS232.Start();
}

void loop()
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
     for (register unsigned char i = 0; i < SENSORS_COUNT; i++)
          SendData[i] = analogRead(i);

     RS232.Send(SendData, sizeof(unsigned) * SENSORS_COUNT);
}
