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

	public:

		SensorWidget(QWidget* parent, unsigned char uID);
		~SensorWidget();

	private:

		const unsigned char ID;

		Ui::SensorWidget* Interface;

		SensorDialog* Dialog;

		QString Equation;
		bool Active = false;

	public slots:

		void onUpdateValue(QScriptEngine& Engine);
		void onOptionsClick(void);
		void onDeleteClick(void);
		void onDialogSave(const SensorDialog::SensorData& tData);

	signals:

		void onValueChange(float fValue);
		void onWidgetDelete(unsigned char uID,
						unsigned char uWT);

};

#endif // SENSORWIDGET_HPP
