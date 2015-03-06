#include "settingsdialog.hpp"
#include "ui_settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent)
: QDialog(parent), Interface(new Ui::SettingsDialog)
{
	Interface->setupUi(this);
}

SettingsDialog::~SettingsDialog()
{
	delete Interface;
}

void SettingsDialog::LoadSettings(const QString& sFile, const QString& sSection)
{
	QSettings INI(sFile, QSettings::IniFormat);

	qDebug() << "section " << sSection;

	INI.beginGroup(sSection);

	const QStringList childKeys = INI.childKeys();

	foreach (const QString &childKey, childKeys)

		qDebug() << INI.value(childKey).toString();

	INI.endGroup();

	qDebug() << "all of sections...";
}

void SettingsDialog::onDialogSave(void)
{
	SettingsDialog::SensorData tData;

	tData.Name = Interface->Name->text();
	tData.Equation = Interface->Equation->text();
	tData.Label = Interface->Type->text();
	tData.Minimum = Interface->Min->value();
	tData.Maximum = Interface->Max->value();
	tData.Active = Interface->Enabled->isChecked();
	tData.Style = Interface->StyleBar->isChecked();

	emit onSettingsAccept(tData);
}
