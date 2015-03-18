#ifndef SENSORWIDGET_HPP
#define SENSORWIDGET_HPP

#include <QWidget>
#include <QScriptEngine>
#include <QRegularExpression>
#include <QScriptValueIterator>
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
			QString Label;

			bool Active = false;
			bool Virtual = false;
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

	private slots:

		void onOptionsClick(void);
		void onDeleteClick(void);
		void onDialogSave(const SensorDialog::SensorData& tData);

	public slots:

		void onUpdateSample(bool bActive, unsigned uSamples);
		void onUpdateValue(QScriptEngine& Engine);

	signals:

		void onDataChange(void);

		void onWidgetDelete(unsigned char uID,
						unsigned char uWT);

};

#endif // SENSORWIDGET_HPP
