#ifndef SENSOR_HPP
#define SENSOR_HPP

#include <QObject>
#include <QString>
#include <QSerialPort>
#include <QMessageBox>
#include <QDebug>

#include "interface/settingsdialog.hpp"

class Sensor : public QObject
{

		Q_OBJECT

	private:

		static unsigned char cLastID;

		static QSerialPort* sSerial;

		const unsigned char ID;

		QString sEquation;

		float fValue;

	public:

		Sensor(void);

		~Sensor(void);

		unsigned char GetID(void) const;

		static void SetDevice(const QString& sPort);

	public slots:

		void SetValue(unsigned uData);

		void SetParams(const SettingsDialog::SensorData& tData);

	signals:

		void onValueChange(float fNewValue);

};

#endif // SENSOR_HPP
