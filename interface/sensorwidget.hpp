#ifndef SENSORWIDGET_HPP
#define SENSORWIDGET_HPP

#include <QWidget>
#include <QScriptEngine>
#include <QRegularExpression>
#include <QDebug>

#include "interface/settingsdialog.hpp"

namespace Ui
{
	class SensorWidget;
}

class SensorWidget : public QWidget
{

		Q_OBJECT

	public:
		SensorWidget(QWidget* parent, int ID);

		~SensorWidget();

	private:

		Ui::SensorWidget* Interface;

		SettingsDialog* dDialog;

		QString Equation;

		bool bActive;

	public slots:

		void onUpdateValue(QScriptEngine* Engine);

		void onOptionsClick(void);

		void onDialogSave(const SettingsDialog::SensorData& tData);

	signals:

		void onValueChange(float fValue);

};

#endif // SENSORWIDGET_HPP
