#include "sensor.hpp"

unsigned char Sensor::cLastID = 0;

Sensor::Sensor(void)
: ID(cLastID++) {}

Sensor::~Sensor(void) {}

unsigned char Sensor::GetID(void) const
{
	return ID;
}

void Sensor::SetValue(unsigned uData)
{
	fValue = ((uData * 5) / 1024.0 - 0.51) / 0.01;

	emit onValueChange(fValue);
}

void Sensor::SetParams(const SettingsDialog::SensorData& tData)
{
	sEquation = tData.Equation;

}
