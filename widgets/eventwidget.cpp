#include "eventwidget.hpp"
#include "ui_eventwidget.h"

#include "windows/mainwindow.hpp"

EventWidget::EventWidget(QWidget* parent, unsigned char uID)
: QWidget(parent), ID(uID), Interface(new Ui::EventWidget)
{
	Interface->setupUi(this);

	Dialog = new EventDialog(this, ID);

	connect(Dialog,
		   SIGNAL(onSettingsAccept(const EventDialog::EventData&)),
		   SLOT(onDialogSave(const EventDialog::EventData&)));

	connect(parent,
		   SIGNAL(onRefreshEvents(QScriptEngine&)),
		   SLOT(onUpdateValue(QScriptEngine&)));

	connect(parent,
		   SIGNAL(onControlChange(unsigned)),
		   SLOT(onUpdateControl(unsigned)));

	Dialog->LoadSettings();
}

EventWidget::~EventWidget()
{
	delete Dialog;
	delete Interface;
}

bool EventWidget::getEventFrame(unsigned char* pcFrame)
{
	if (!Data.Active || !Data.Simple) return false;

	pcFrame[1] = (Data.Action << 4) + Data.Grow ? Data.When : !Data.When;
	pcFrame[2] = ((Data.Pin - 2) << 4) + Data.Sensor;
	pcFrame[3] = (unsigned char) (Data.Value >> 8);
	pcFrame[4] = (unsigned char) Data.Value;

	return true;
}

void EventWidget::onUpdateValue(QScriptEngine& Engine)
{
	if (!Data.Active) return;

	float fValue = Engine.globalObject().property(Data.Variable).toNumber();

	switch (Data.When)
	{
		case true:
			if ((fValue > Data.SwitchValue && Data.SwitchValue > Data.LastValue) ||
			    (fValue > Data.SwitchValue && Data.Switch))
			{
				Data.Switch = false;

				emit onSwitchEvent(Data.Pin, Data.Action);
			}
			Data.LastValue = fValue;
		break;
		case false:
			if ((fValue < Data.SwitchValue && Data.SwitchValue < Data.LastValue) ||
			    (fValue < Data.SwitchValue && Data.Switch))
			{
				Data.Switch = false;

				emit onSwitchEvent(Data.Pin, Data.Action);
			}
			Data.LastValue = fValue;
		break;
	}
}

void EventWidget::onUpdateControl(unsigned uControl)
{
	Interface->Name->setEnabled(!(uControl & 8) || Data.Simple);

	Data.Switch = true;
}

void EventWidget::onUpdateData(void)
{
	Dialog->LoadSettings();
}

void EventWidget::onOptionsClick(void)
{
	Dialog->open();
}

void EventWidget::onDeleteClick(void)
{

	if (QMessageBox::Yes == QMessageBox::question(
		    this,
		    "Usuń sensor",
		    "Czy chcesz bezpowrotnie usunąć to zdarzenie?",
		    QMessageBox::Yes|QMessageBox::No))
	{
		if (Dialog->DeleteSettings())
			emit onWidgetDelete(ID, TYPE_EVENT);
		else
			QMessageBox::warning(
				this,
				"Błąd",
				"Nie udało się usunąć zdarzenia");
	}
}

void EventWidget::onDialogSave(const EventDialog::EventData& tData)
{
	Interface->Name->setEnabled(tData.Active);
	Interface->Desc->setEnabled(tData.Active);

	Interface->Name->setText(tData.Name);

	Interface->Desc->setText(
				QString("%1 -> %2")
				.arg(tData.Sensor)
				.arg(tData.Device));

	Interface->Desc->setToolTip(
		QString("%1 urządzenie \"%2\" gdy wartość czujnika \"%3\" będzie %4 od %5")
		.arg(tData.Action ? "Włącz" : "Wyłącz")
		.arg(tData.Device)
		.arg(tData.Sensor)
		.arg(tData.Where ? "większa" : "mniejsza")
		.arg(tData.Value));

	Data.Variable = tData.Variable;

	Data.Pin = tData.PinID;
	Data.Sensor = tData.SeID;

	Data.Value = tData.Voltage * (1024.0 / 5.0);

	Data.SwitchValue = tData.Value;

	Data.Active = tData.Active;
	Data.Action = tData.Action;
	Data.Simple = tData.Simple;
	Data.When = tData.Where;
	Data.Grow = tData.Grow;

	Data.Switch = true;

	emit onEventRefresh();
}
