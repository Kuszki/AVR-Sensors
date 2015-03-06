#ifndef SENSORWIDGET_HPP
#define SENSORWIDGET_HPP

#include <QWidget>

#include "settingsdialog.hpp"

#include "sensor/sensor.hpp"

namespace Ui
{
	class SensorWidget;
}

class SensorWidget : public QWidget
{

		Q_OBJECT

	public:
		explicit SensorWidget(QWidget *parent = 0);
		~SensorWidget();

	private:

		Ui::SensorWidget* Interface;

		SettingsDialog* dDialog;

		Sensor* sSensor;

		bool bActive;

	public slots:

		void UpdateValue(unsigned uValue);

		void SetValue(float fValue);

		void SetLabel(const QString& sLabel);

		void onOptionsClick(void);

		void onDialogSave(const SettingsDialog::SensorData& tData);

};

#endif // SENSORWIDGET_HPP
