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

	Dialog->LoadSettings();
}

DeviceWidget::~DeviceWidget()
{
	delete Interface;
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
	Interface->Enabled->setChecked(tData.Active);
	Interface->Pin->setText(QString::number(tData.Pin));

	emit onDataChange();
}
