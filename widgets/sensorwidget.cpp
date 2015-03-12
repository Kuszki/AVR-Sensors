#include "sensorwidget.hpp"
#include "ui_sensorwidget.h"

SensorWidget::SensorWidget(QWidget *parent, int ID)
: QWidget(parent), Interface(new Ui::SensorWidget)
{
	Interface->setupUi(this);

	dDialog = new SensorDialog(this);

	connect(dDialog, SIGNAL(onSettingsAccept(const SensorDialog::SensorData&)),
		   SLOT(onDialogSave(const SensorDialog::SensorData&)));

	dDialog->LoadSettings(QString::number(ID));

	connect(parent, SIGNAL(onRefresh(QScriptEngine*)), SLOT(onUpdateValue(QScriptEngine*)));
}

SensorWidget::~SensorWidget()
{
	delete Interface;
	delete dDialog;
}

void SensorWidget::onUpdateValue(QScriptEngine* Engine)
{
	if (bActive)
	{
		float fValue = Engine->evaluate(Equation).toNumber();

		Interface->Progress->setValue(fValue);
		Interface->Value->display(fValue);

		emit onValueChange(fValue);
	}
}

void SensorWidget::onOptionsClick(void)
{
	dDialog->open();
}

void SensorWidget::onDialogSave(const SensorDialog::SensorData& tData)
{
	bActive = tData.Active;

	if (!bActive)
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

	Equation = tData.Equation;
}
