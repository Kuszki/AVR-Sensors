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

	Dialog->LoadSettings();
}

EventWidget::~EventWidget()
{
	delete Dialog;
	delete Interface;
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
		if (Dialog->DeleteSettings()) emit onWidgetDelete(ID, TYPE_EVENT);
		else QMessageBox::warning(
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
				QString("%1 %2 gdy wartość %3 %4 od %5")
				.arg(tData.Action ? "Włącz" : "Wyłącz")
				.arg(tData.Device)
				.arg(tData.Sensor)
				.arg(tData.Where ? "większa" : "mniejsza")
				.arg(tData.Value));

	// TODO: emit event
}
