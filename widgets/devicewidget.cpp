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
		   SIGNAL(onControlChange(unsigned)),
		   SLOT(onChangeControl(unsigned)));

	connect(parent,
		   SIGNAL(onDeviceUpdate(unsigned char, bool)),
		   SLOT(onChangeState(unsigned char,bool)));

	connect(parent,
		   SIGNAL(onRefreshDevices(unsigned char)),
		   SLOT(onChangeState(unsigned char)));

	Dialog->LoadSettings();
}

DeviceWidget::~DeviceWidget()
{
	delete Interface;
}

bool DeviceWidget::getDeviceFrame(unsigned char& uFrame)
{
	uFrame |= bDefaultState << (uPin - 2);

	return true;
}

void DeviceWidget::onActiveSwitch(bool bMode)
{
	emit onManualSwitch(uPin, bMode);
}

void DeviceWidget::onChangeControl(unsigned uControl)
{
	bool bManual = uControl & 2;

	Interface->Enabled->setEnabled(bManual);

	if (bManual)
		emit onManualSwitch(uPin, Interface->Enabled->isChecked());
}

void DeviceWidget::onChangeState(unsigned char uPinID, bool bState)
{
	if (uPinID == uPin)
		Interface->Enabled->setChecked(bState);
}

void DeviceWidget::onChangeState(unsigned char uPins)
{
	Interface->Enabled->setChecked(uPins & (1 << (uPin - 2)));
}

void DeviceWidget::onOptionsClick(void)
{
	Dialog->open();
}

void DeviceWidget::onDeleteClick(void)
{

	if (QMessageBox::Yes ==
	    QMessageBox::question(
		    this,
		    "Usuń sensor",
		    "Czy chcesz bezpowrotnie usunąć to urządzenie?",
		    QMessageBox::Yes|QMessageBox::No))
	{
		if (Dialog->DeleteSettings())
			emit onWidgetDelete(ID, TYPE_DEVICE);
		else
			QMessageBox::warning(
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

	bDefaultState = tData.Active;

	uPin = tData.Pin;

	emit onDataChange();
}
