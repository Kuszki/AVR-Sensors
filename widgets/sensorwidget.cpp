#include "sensorwidget.hpp"
#include "ui_sensorwidget.h"

#include "windows/mainwindow.hpp"

SensorWidget::SensorWidget(QWidget *parent, unsigned char uID)
: QWidget(parent), ID(uID), Interface(new Ui::SensorWidget)
{
	Interface->setupUi(this);

	Dialog = new SensorDialog(this, ID);

	connect(Dialog,
		   SIGNAL(onSettingsAccept(const SensorDialog::SensorData&)),
		   SLOT(onDialogSave(const SensorDialog::SensorData&)));

	connect(parent,
		   SIGNAL(onRefreshValues(QScriptEngine&)),
		   SLOT(onUpdateValue(QScriptEngine&)));

	connect(parent,
		   SIGNAL(onSampleUpdate(bool, unsigned)),
		   SLOT(onUpdateSample(bool, unsigned)));

	Dialog->LoadSettings();
}

SensorWidget::~SensorWidget()
{
	delete Dialog;
	delete Interface;
}

void SensorWidget::onUpdateSample(bool bActive, unsigned uSamples)
{
	if (Samples.pfSamples) delete [] Samples.pfSamples;

	if (bActive)
	{
		Samples.pfSamples = new double[uSamples];

		Samples.uSamples = uSamples;
	}
	else
	{
		Samples.pfSamples = nullptr;

		Samples.uSamples = 0;
	}

	Samples.uCurrent = 0;
}

void SensorWidget::onUpdateValue(QScriptEngine& Engine)
{
	if (Data.Active)
	{
		double fValue = Engine.evaluate(Data.Equation).toNumber();

		if (Samples.uSamples == Samples.uCurrent)
		{
			if (Samples.uSamples) fValue = AVG(Samples);

			Interface->Progress->setValue(fValue);
			Interface->Value->display(fValue);
		}
		else
		{
			Samples.pfSamples[Samples.uCurrent++] = fValue;
		}
	}
}

void SensorWidget::onOptionsClick(void)
{
	Dialog->open();
}

void SensorWidget::onDeleteClick(void)
{

	if (QMessageBox::Yes == QMessageBox::question(
		    this,
		    "Usuń sensor",
		    "Czy chcesz bezpowrotnie usunąć ten sensor?",
		    QMessageBox::Yes|QMessageBox::No))
	{
		if (Dialog->DeleteSettings()) emit onWidgetDelete(ID, TYPE_SENSOR);
		else QMessageBox::warning(
					this,
					"Błąd",
					"Nie udało się usunąć sensora");
	}
}

void SensorWidget::onDialogSave(const SensorDialog::SensorData& tData)
{
	Data.Active = tData.Active;

	if (!Data.Active)
	{
		Interface->Progress->setValue(0);
		Interface->Value->display(0);
	}

	Interface->Desc->setText(tData.Label);

	Interface->Label->setText(tData.Name);
	Interface->Label->setEnabled(tData.Active);

	Interface->Progress->setVisible(tData.Style && tData.Active);
	Interface->Progress->setFormat(tData.Label.isEmpty() ? "%v [%p%]" : "%v [" + tData.Label + "]");
	Interface->Progress->setMaximum(tData.Maximum);
	Interface->Progress->setMinimum(tData.Minimum);

	Interface->Value->setVisible(!tData.Style && tData.Active);

	Interface->Desc->setVisible(!tData.Style && tData.Active);

	Interface->Label->setSizePolicy(tData.Active ? QSizePolicy::Preferred : QSizePolicy::Expanding, QSizePolicy::Preferred);

	Data.Equation = tData.Equation;

	emit onDataChange();
}

double SensorWidget::AVG(Measure& tData)
{
	double Buff = 0.0;

	for (int i = 0; i < tData.uSamples; i++) Buff += tData.pfSamples[i];

	tData.uCurrent = 0;

	return Buff / tData.uSamples;
}
