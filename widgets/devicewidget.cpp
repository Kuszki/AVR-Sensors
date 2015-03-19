#include "devicewidget.hpp"
#include "ui_devicewidget.h"

#include "windows/mainwindow.hpp"

DeviceWidget::DeviceWidget(QWidget* parent, unsigned char uID)
: QWidget(parent), ID(uID), Interface(new Ui::DeviceWidget)
{
	Interface->setupUi(this);

	Dialog = new DeviceDialog(this, ID);

	connect(Dialog,
		   SIGNAL(onSettingsAccept(const DeviceDialog::DeviceData&)),
		   SLOT(onDialogSave(const DeviceDialog::DeviceData&)));

	connect(parent,
		   SIGNAL(onControlChange(bool)),
		   SLOT(onChangeControl(bool)));

	connect(parent,
		   SIGNAL(onDeviceUpdate(unsigned char, bool)),
		   SLOT(onChangeState(unsigned char,bool)));

	Dialog->LoadSettings();
}

DeviceWidget::~DeviceWidget()
{
	delete Interface;
}

void DeviceWidget::onActiveSwitch(bool bMode)
{
	emit onManualSwitch(uPin, bMode);
}

void DeviceWidget::onChangeControl(bool bManual)
{
	Interface->Enabled->setEnabled(bManual);

	if (bManual)
		emit onManualSwitch(uPin, Interface->Enabled->isChecked());
}

void DeviceWidget::onChangeState(unsigned char uPinID, bool bState)
{
	if (uPinID == uPin)
		Interface->Enabled->setChecked(bState);
}

void DeviceWidget::onOptionsClick(void)
{
	Dialog->open();
}

void DeviceWidget::onDeleteClick(void)
{

	if (QMessageBox::Yes == QMessageBox::question(
		    this,
		    "Usuń sensor",
		    "Czy chcesz bezpowrotnie usunąć to urządzenie?",
		    QMessageBox::Yes|QMessageBox::No))
	{
		if (Dialog->DeleteSettings()) emit onWidgetDelete(ID, TYPE_DEVICE);
		else QMessageBox::warning(
					this,
					"Błąd",
					"Nie udało się usunąć urządzenia");
	}
}

void DeviceWidget::onDialogSave(const DeviceDialog::DeviceData& tData)
{
	Interface->Name->setText(tData.Name);
	Interface->Pin->setText(QString::number(tData.Pin));

	if (!bInitialSwitch)
	{
		Interface->Enabled->setChecked(tData.Active);

		bInitialSwitch = true;
	}

	uPin = tData.Pin;

	emit onDataChange();
}
