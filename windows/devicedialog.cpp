#include "devicedialog.hpp"
#include "ui_devicedialog.h"

#include "windows/mainwindow.hpp"

DeviceDialog::DeviceDialog(QWidget* parent, unsigned char uID)
: QDialog(parent), ID(uID), Interface(new Ui::DeviceDialog)
{
	Interface->setupUi(this);

	if (!ID) setWindowTitle("Dodaj urządzenie");

	GetData(DefaultData);
}

DeviceDialog::~DeviceDialog()
{
	delete Interface;
}

bool DeviceDialog::LoadSettings(void)
{
	QSqlQuery query(MainWindow::getInstance()->getDatabase());

	query.prepare("SELECT name, pin, enabled \
			    FROM targets \
			    WHERE ID=:ID");

	query.bindValue(":ID", ID);

	if (query.exec() && query.next())
	{
		LastData.Name = query.value(0).toString();
		LastData.Pin = query.value(1).toUInt();
		LastData.Active = query.value(2).toBool();
	}
	else	return false;

	SetData(LastData, true);

	return true;
}

bool DeviceDialog::SaveSettings(void)
{
	QSqlQuery query(MainWindow::getInstance()->getDatabase());

	if (ID) query.prepare(
				"UPDATE targets SET \
				name=:name, \
				pin=:pin, \
				enabled=:enabled \
				WHERE ID=:ID");
	else	query.prepare(
				"INSERT INTO targets \
				(name, pin, enabled) \
				VALUES \
				(:name, :pin, :enabled)");

	GetData(LastData);

	query.bindValue(":name", LastData.Name);
	query.bindValue(":pin", LastData.Pin);
	query.bindValue(":enabled", LastData.Active);
	query.bindValue(":ID", ID);

	if (!query.exec()) return false;

	if (ID)
		emit onSettingsAccept(LastData);
	else
		emit onWidgetAdd(query.lastInsertId().toUInt());

	return true;
}

bool DeviceDialog::DeleteSettings(void)
{
	QSqlQuery query(MainWindow::getInstance()->getDatabase());

	query.prepare("SELECT \
			    count(ID) \
			    FROM events WHERE \
			    TARGET_ID=:ID");

	query.bindValue(":ID", ID);

	if (!query.exec() || (query.next() && query.value(0).toBool())) return false;

	query.prepare("DELETE FROM targets WHERE \
			    ID=:ID");

	query.bindValue(":ID", ID);

	return query.exec();
}

void DeviceDialog::GetData(DeviceData& tData)
{
	tData.Name = Interface->Name->text();
	tData.Pin = Interface->Pin->value();
	tData.Active = Interface->Active->currentIndex();
}

void DeviceDialog::SetData(DeviceData& tData, bool bRefresh)
{
	Interface->Name->setText(tData.Name);
	Interface->Pin->setValue(tData.Pin);
	Interface->Active->setCurrentIndex(tData.Active);

	if (bRefresh) emit onSettingsAccept(tData);
}

void DeviceDialog::open(void)
{
	if (!ID) SetData(DefaultData, false);

	QDialog::open();
}

void DeviceDialog::accept(void)
{
	if (!SaveSettings()) QMessageBox::warning(
				this,
				"Błąd",
				"Nie udało się zapisać rekordu w bazie danych");
	else QDialog::accept();
}

void DeviceDialog::reject(void)
{
	QDialog::reject();

	SetData(LastData, false);
}

void DeviceDialog::onParamsChange(void)
{
	Interface->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(
				!Interface->Name->text().isEmpty()
				);
}
