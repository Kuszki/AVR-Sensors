#include "sensorwidget.hpp"
#include "ui_sensorwidget.h"

SensorWidget::SensorWidget(QWidget *parent)
: QWidget(parent), Interface(new Ui::SensorWidget)
{
	Interface->setupUi(this);

	dDialog = new SettingsDialog(this);
	sSensor = new Sensor();

	connect(dDialog, SIGNAL(onSettingsAccept(const SettingsDialog::SensorData&)),
		   SLOT(onDialogSave(const SettingsDialog::SensorData&)));

	connect(dDialog, SIGNAL(onSettingsAccept(const SettingsDialog::SensorData&)),
		   sSensor, SLOT(SetParams(const SettingsDialog::SensorData&)));

	connect(sSensor, SIGNAL(onValueChange(float)), SLOT(SetValue(float)));

	dDialog->LoadSettings("Sensors.ini", QString::number(sSensor->GetID()));
}

SensorWidget::~SensorWidget()
{
	delete Interface;
	delete dDialog;
	delete sSensor;
}

void SensorWidget::UpdateValue(unsigned uValue)
{
	if (bActive) sSensor->SetValue(uValue);
}

void SensorWidget::SetValue(float fValue)
{
	Interface->Progress->setValue(fValue);
	Interface->Value->display(fValue);
}

void SensorWidget::SetLabel(const QString &sLabel)
{
	Interface->Label->setText(sLabel);
}

void SensorWidget::onOptionsClick(void)
{
	dDialog->open();
}

void SensorWidget::onDialogSave(const SettingsDialog::SensorData& tData)
{
	bActive = tData.Active;

	Interface->Label->setText(tData.Name);
	Interface->Label->setEnabled(tData.Active);

	Interface->Progress->setVisible(tData.Style);
	Interface->Progress->setFormat("%v [" + tData.Label + "]");
	Interface->Progress->setMaximum(tData.Maximum);
	Interface->Progress->setMinimum(tData.Minimum);

	Interface->Value->setVisible(!tData.Style);
}
