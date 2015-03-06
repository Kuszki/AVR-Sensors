#include "settingsdialog.hpp"
#include "ui_settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent)
: QDialog(parent), Interface(new Ui::SettingsDialog)
{
	Interface->setupUi(this);
}

SettingsDialog::~SettingsDialog()
{
	SaveSettings();

	delete Interface;
}

void SettingsDialog::LoadSettings(const QString& sSection)
{
	if (!sSection.isEmpty()) sSensor = sSection;

	if (sSensor.isEmpty()) return;

	QSettings INI("Sensors.ini", QSettings::IniFormat);

	INI.setIniCodec("UTF-8");
	INI.beginGroup(sSensor);

	tLastData.Name = INI.value("Name", "Czujnik " + sSection).toString();
	tLastData.Equation = INI.value("Equation", "x").toString();
	tLastData.Label = INI.value("Label", "").toString();
	tLastData.Minimum = INI.value("Minimum", 0).toInt();
	tLastData.Maximum = INI.value("Maximum", 5).toInt();
	tLastData.Active = INI.value("Active", false).toBool();
	tLastData.Style = INI.value("Style", false).toBool();

	SetData(tLastData);
}

void SettingsDialog::SaveSettings(const QString& sSection)
{
	if (!sSection.isEmpty()) sSensor = sSection;

	QSettings INI("Sensors.ini", QSettings::IniFormat);

	INI.setIniCodec("UTF-8");
	INI.beginGroup(sSensor);

	GetData(tLastData);

	INI.setValue("Active", tLastData.Active);
	INI.setValue("Equation", tLastData.Equation);
	INI.setValue("Label", tLastData.Label);
	INI.setValue("Maximum", tLastData.Maximum);
	INI.setValue("Minimum", tLastData.Minimum);
	INI.setValue("Name", tLastData.Name);
	INI.setValue("Style", tLastData.Style);
}

void SettingsDialog::GetData(SettingsDialog::SensorData& tData)
{
	tData.Name = Interface->Name->text();
	tData.Equation = Interface->Equation->text();
	tData.Label = Interface->Type->text();
	tData.Minimum = Interface->Min->value();
	tData.Maximum = Interface->Max->value();
	tData.Active = Interface->Enabled->isChecked();
	tData.Style = Interface->StyleBar->isChecked();
}

void SettingsDialog::SetData(SettingsDialog::SensorData& tData, bool bRefresh)
{
	Interface->Name->setText(tData.Name);
	Interface->Equation->setText(tData.Equation);
	Interface->Type->setText(tData.Label);
	Interface->Min->setValue(tData.Minimum);
	Interface->Max->setValue(tData.Maximum);
	Interface->Enabled->setChecked(tData.Active);
	Interface->StyleBar->setChecked(tData.Style);
	Interface->StyleLCD->setChecked(!tData.Style);

	if (bRefresh) emit onSettingsAccept(tData);
}

void SettingsDialog::accept(void)
{
	QDialog::accept();

	GetData(tLastData);

	emit onSettingsAccept(tLastData);
}

void SettingsDialog::reject(void)
{
	QDialog::reject();

	SetData(tLastData, false);
}

void SettingsDialog::onParamsChange(void)
{
	Interface->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(
				Interface->Min->value() < Interface->Max->value() &&
				!Interface->Name->text().isEmpty() &&
				!Interface->Equation->text().isEmpty()
				);
}
