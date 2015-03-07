#ifndef SENSORWIDGET_HPP
#define SENSORWIDGET_HPP

#include <QWidget>
#include <QScriptEngine>
#include <QRegularExpression>
#include <QDebug>

#include "settingsdialog.hpp"

namespace Ui
{
	class SensorWidget;
}

class SensorWidget : public QWidget
{

		Q_OBJECT

	public:
		SensorWidget(QWidget* parent,
				   unsigned char uID);

		~SensorWidget();

	private:

		const unsigned char ID;

		Ui::SensorWidget* Interface;

		SettingsDialog* dDialog;

		QScriptEngine Engine;

		QString Equation;

		bool bActive;

	public slots:

		void onUpdateValue(unsigned uValue);

		void onOptionsClick(void);

		void onDialogSave(const SettingsDialog::SensorData& tData);

	signals:

		void onValueChange(float fValue);

};

#endif // SENSORWIDGET_HPP
