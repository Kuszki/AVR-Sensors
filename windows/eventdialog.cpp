#include "eventdialog.hpp"
#include "ui_eventdialog.h"

#include "windows/mainwindow.hpp"

EventDialog::EventDialog(QWidget *parent, unsigned char uID)
: QDialog(parent), ID(uID), Interface(new Ui::EventDialog)
{
	Interface->setupUi(this);

	if (!ID) setWindowTitle("Dodaj zdarzenie");

	GetData(DefaultData);
}

EventDialog::~EventDialog()
{
	delete Interface;
}

bool EventDialog::LoadSettings(void)
{
	QSqlQuery query(MainWindow::getInstance()->getDatabase());

	query.prepare(
		"SELECT name, value, condition, state, active, SENSOR_ID, TARGET_ID \
		FROM events \
		WHERE ID=:ID");

	query.bindValue(":ID", ID);

	if (query.exec() && query.next())
	{
		LastData.Name = query.value(0).toString();
		LastData.Value = query.value(1).toDouble();
		LastData.Where = query.value(2).toBool();
		LastData.Action = query.value(3).toBool();
		LastData.Active = query.value(4).toBool();
		LastData.SensorID = query.value(5).toUInt();
		LastData.DeviceID = query.value(6).toUInt();
	}
	else	return false;

	query.prepare("SELECT name FROM sensors WHERE ID=:ID");

	query.bindValue(":ID", LastData.SensorID);

	if (query.exec() && query.next())
	{
		LastData.Sensor = query.value(0).toString();
	}
	else return false;

	query.prepare("SELECT name FROM targets WHERE ID=:ID");

	query.bindValue(":ID", LastData.DeviceID);

	if (query.exec() && query.next())
	{
		LastData.Device = query.value(0).toString();
	}
	else	return false;

	SetData(LastData, true);

	return true;
}

bool EventDialog::SaveSettings(void)
{
	QSqlQuery query(MainWindow::getInstance()->getDatabase());

	if (ID) query.prepare(
		"UPDATE events SET \
		name=:name, \
		value=:value, \
		condition=:condition, \
		state=:state, \
		active=:active, \
		SENSOR_ID=:SENSOR_ID, \
		TARGET_ID=:TARGET_ID \
		WHERE ID=:ID");
	else query.prepare(
		"INSERT INTO events \
		(name, value, condition, state, active, SENSOR_ID, TARGET_ID) \
		VALUES \
		(:name, :value, :condition, :state, :active, :SENSOR_ID, :TARGET_ID)");

	GetData(LastData);

	query.bindValue(":name", LastData.Name);
	query.bindValue(":value", LastData.Value);
	query.bindValue(":condition", LastData.Where);
	query.bindValue(":state", LastData.Action);
	query.bindValue(":active", LastData.Active);
	query.bindValue(":SENSOR_ID", LastData.SensorID);
	query.bindValue(":TARGET_ID", LastData.DeviceID);
	query.bindValue(":ID", ID);

	if (!query.exec()) return false;

	if (ID)
		emit onSettingsAccept(LastData);
	else
		emit onWidgetAdd(query.lastInsertId().toUInt());

	return true;
}

bool EventDialog::DeleteSettings(void)
{
	QSqlQuery query(MainWindow::getInstance()->getDatabase());

	query.prepare("DELETE FROM events WHERE \
			    ID=:ID");

	query.bindValue(":ID", ID);

	return query.exec();
}

void EventDialog::GetData(EventData& tData)
{
	tData.Name = Interface->Name->text();
	tData.Value = Interface->Value->value();
	tData.Where = Interface->Where->currentIndex();
	tData.Action = Interface->Action->currentIndex();
	tData.Active = Interface->Active->isChecked();
	tData.Sensor = Interface->Sensor->currentText();
	tData.Device = Interface->Device->currentText();
	tData.SensorID = Interface->Sensor->currentData().toUInt();
	tData.DeviceID = Interface->Device->currentData().toUInt();
}

void EventDialog::SetData(EventData& tData, bool bRefresh)
{
	Interface->Name->setText(tData.Name);
	Interface->Value->setValue(tData.Value);
	Interface->Where->setCurrentIndex(tData.Where);
	Interface->Action->setCurrentIndex(tData.Action);
	Interface->Active->setChecked(tData.Active);

	Interface->Sensor->setCurrentIndex(
				Interface->Sensor->findData(tData.SensorID));
	Interface->Device->setCurrentIndex(
				Interface->Device->findData(tData.DeviceID));

	if (bRefresh) emit onSettingsAccept(tData);
}

void EventDialog::open(void)
{
	QSqlQuery query(MainWindow::getInstance()->getDatabase());

	if (!ID) SetData(DefaultData, false);

	Interface->Sensor->clear();
	Interface->Device->clear();

	query.prepare("SELECT ID, name FROM sensors");

	if (query.exec()) while (query.next())
		Interface->Sensor->addItem(
					query.value(1).toString(),
					query.value(0).toUInt());

	query.prepare("SELECT ID, name FROM targets");

	if (query.exec()) while (query.next())
		Interface->Device->addItem(
					query.value(1).toString(),
					query.value(0).toUInt());

	Interface->Sensor->setCurrentIndex(
				Interface->Sensor->findData(LastData.SensorID));
	Interface->Device->setCurrentIndex(
				Interface->Device->findData(LastData.DeviceID));

	QDialog::open();
}

void EventDialog::accept(void)
{
	if (!SaveSettings())
	{
		QMessageBox::warning(
					this,
					"Błąd",
					"Nie udało się zapisać rekordu w bazie danych");
	}
	else QDialog::accept();
}

void EventDialog::reject(void)
{
	QDialog::reject();

	SetData(LastData, false);
}

void EventDialog::onParamsChange(void)
{
	Interface->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(
				!Interface->Name->text().isEmpty()
				);
}
