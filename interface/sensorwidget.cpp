#include "sensorwidget.hpp"
#include "ui_sensorwidget.h"

SensorWidget::SensorWidget(QWidget *parent, unsigned char uID)
: QWidget(parent), ID(uID), Interface(new Ui::SensorWidget)
{
	Interface->setupUi(this);

	dDialog = new SettingsDialog(this);

	connect(dDialog, SIGNAL(onSettingsAccept(const SettingsDialog::SensorData&)),
		   SLOT(onDialogSave(const SettingsDialog::SensorData&)));

	dDialog->LoadSettings(QString::number(ID));
}

SensorWidget::~SensorWidget()
{
	delete Interface;
	delete dDialog;
}

void SensorWidget::onUpdateValue(unsigned uValue)
{
	if (bActive)
	{
		QScriptEngine Script;

		float fValue = (uValue * 5) / 1024.0;

		QString equation = Formula;

		equation.replace(QRegularExpression("x"),
					  QString::number(fValue));

		fValue = Script.evaluate(equation).toNumber();

		Interface->Progress->setValue(fValue);
		Interface->Value->display(fValue);

		emit onValueChange(fValue);
	}
}

void SensorWidget::onOptionsClick(void)
{
	dDialog->open();
}

void SensorWidget::onDialogSave(const SettingsDialog::SensorData& tData)
{
	bActive = tData.Active;

	const QString Label = tData.Label.isEmpty() ? "%v [%p%]" : "%v [" + tData.Label + "]";

	Interface->Desc->setText(tData.Label);

	Interface->Label->setText(tData.Name);
	Interface->Label->setEnabled(tData.Active);

	Interface->Progress->setVisible(tData.Style);
	Interface->Progress->setFormat(Label);
	Interface->Progress->setMaximum(tData.Maximum);
	Interface->Progress->setMinimum(tData.Minimum);

	Interface->Value->setVisible(!tData.Style);

	Interface->Desc->setVisible(!tData.Style);

	Formula = tData.Equation;
}
