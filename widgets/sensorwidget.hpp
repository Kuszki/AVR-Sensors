#ifndef SENSORWIDGET_HPP
#define SENSORWIDGET_HPP

#include <QWidget>
#include <QScriptEngine>
#include <QRegularExpression>
#include <QDebug>

#include "windows/sensordialog.hpp"

namespace Ui
{
	class SensorWidget;
}

class SensorWidget : public QWidget
{

		Q_OBJECT

	private:

		struct Measure
		{
			unsigned uSamples = 0;
			unsigned uCurrent = 0;

			double* pfSamples = nullptr;
		};

		struct Sensor
		{
			QString Equation;

			bool Active = false;
		};

		const unsigned char ID;

		Ui::SensorWidget* Interface;

		SensorDialog* Dialog;

		Measure Samples;
		Sensor Data;

	public:

		SensorWidget(QWidget* parent, unsigned char uID);
		~SensorWidget();

		static double AVG(Measure& tData);

	public slots:

		void onUpdateSample(bool bActive, unsigned uSamples);
		void onUpdateValue(QScriptEngine& Engine);
		void onOptionsClick(void);
		void onDeleteClick(void);
		void onDialogSave(const SensorDialog::SensorData& tData);

	signals:

		void onDataChange(void);
		void onWidgetDelete(unsigned char uID,
						unsigned char uWT);

};

#endif // SENSORWIDGET_HPP
