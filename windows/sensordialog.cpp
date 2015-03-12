#include "sensordialog.hpp"
#include "ui_sensordialog.h"

SensorDialog::SensorDialog(QWidget *parent)
: QDialog(parent), Interface(new Ui::SensorDialog)
{
	Interface->setupUi(this);
}

SensorDialog::~SensorDialog()
{
	SaveSettings();

	delete Interface;
}

void SensorDialog::LoadSettings(const QString& sSection)
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

void SensorDialog::SaveSettings(const QString& sSection)
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

void SensorDialog::GetData(SensorDialog::SensorData& tData)
{
	tData.Name = Interface->Name->text();
	tData.Equation = Interface->Equation->document()->toPlainText();
	tData.Label = Interface->Type->text();
	tData.Minimum = Interface->Min->value();
	tData.Maximum = Interface->Max->value();
	tData.Active = Interface->Enabled->isChecked();
	tData.Style = Interface->StyleBar->isChecked();
}

void SensorDialog::SetData(SensorDialog::SensorData& tData, bool bRefresh)
{
	Interface->Name->setText(tData.Name);
	Interface->Equation->document()->setPlainText(tData.Equation);
	Interface->Type->setText(tData.Label);
	Interface->Min->setValue(tData.Minimum);
	Interface->Max->setValue(tData.Maximum);
	Interface->Enabled->setChecked(tData.Active);
	Interface->StyleBar->setChecked(tData.Style);
	Interface->StyleLCD->setChecked(!tData.Style);

	if (bRefresh) emit onSettingsAccept(tData);
}

void SensorDialog::accept(void)
{
	const QString& Equation = Interface->Equation->document()->toPlainText();

	QScriptEngine Script;

	QScriptValue Result = Script.evaluate("x1=x2=x3=x4=x5=x6=1;" + Equation);

	if (Equation.isEmpty())
	{
		QMessageBox::warning(this,
						 "Błąd",
						 "Podaj równanie czujnika"
						 );
	}
	else if (Script.hasUncaughtException())
	{
		const QString sLine( QString::number(Script.uncaughtExceptionLineNumber()));
		const QString sError(Result.toString());

		QMessageBox::warning(this,
						 "Błąd",
						 "Podane równanie zawiera błąd w linii " + sLine + "\n\"" + sError + "\""
						 );

	}
	else
	{

		QDialog::accept();

		GetData(tLastData);

		emit onSettingsAccept(tLastData);

	}
}

void SensorDialog::reject(void)
{
	QDialog::reject();

	SetData(tLastData, false);
}

void SensorDialog::onParamsChange(void)
{
	Interface->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(
				Interface->Min->value() < Interface->Max->value() &&
				!Interface->Name->text().isEmpty() &&
				!Interface->Equation->document()->toPlainText().isEmpty()
				);
}
