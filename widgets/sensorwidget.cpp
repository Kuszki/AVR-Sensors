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
		   SIGNAL(onRefresh(QScriptEngine&)),
		   SLOT(onUpdateValue(QScriptEngine&)));

	Dialog->LoadSettings();
}

SensorWidget::~SensorWidget()
{
	delete Dialog;
	delete Interface;
}

void SensorWidget::onUpdateValue(QScriptEngine& Engine)
{
	if (Active)
	{
		float fValue = Engine.evaluate(Equation).toNumber();

		Interface->Progress->setValue(fValue);
		Interface->Value->display(fValue);

		emit onValueChange(fValue);
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
		Dialog->DeleteSettings();

		emit onWidgetDelete(ID, TYPE_SENSOR);
	}
}

void SensorWidget::onDialogSave(const SensorDialog::SensorData& tData)
{
	Active = tData.Active;

	if (!Active)
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
