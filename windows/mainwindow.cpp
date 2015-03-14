#include "mainwindow.hpp"
#include "ui_mainwindow.h"

MainWindow* MainWindow::Instance = nullptr;

MainWindow::MainWindow(QWidget *parent)
: QMainWindow(parent), Interface(new Ui::MainWindow)
{
	if (Instance) qFatal("Próba zduplikowanie głównego okna.");
	else Instance = this;

	Interface->setupUi(this);

	DialogAddSensor = new SensorDialog(this, 0);
	DialogAddEvent = new EventDialog(this, 0);
	DialogAddDevice = new DeviceDialog(this, 0);

	Database = QSqlDatabase::addDatabase("QSQLITE");

	Serial.setBaudRate(QSerialPort::Baud9600);
	Serial.setParity(QSerialPort::NoParity);
	Serial.setStopBits(QSerialPort::OneStop);

	QSettings INI("Sensors.ini", QSettings::IniFormat);

	INI.setIniCodec("UTF-8");

	INI.beginGroup("Connection");

	Interface->Active->setChecked(INI.value("Active", false).toBool());
	Interface->Time->setValue(INI.value("Time", 3.0).toDouble());

	INI.beginGroup("Database");

	const QString dbPath = INI.value("Path", QDir::currentPath() + QDir::separator() + "database.sqlite").toString();

	INI.endGroup();

	if (!QFile::exists(dbPath))
	{
		QFile::copy(":/data/database", dbPath);
		QFile::setPermissions(dbPath, QFile::ReadOwner | QFile::WriteOwner | QFile::ReadGroup | QFile::WriteGroup | QFile::ReadOther);
	}

	Database.setDatabaseName(dbPath);
	Database.open();

	QSqlQuery Query(Database);

	if (Query.exec("SELECT ID FROM sensors"))
		while (Query.next()) AddSensor(Query.value(0).toUInt());

	if (Query.exec("SELECT ID FROM events"))
		while (Query.next()) AddEvent(Query.value(0).toUInt());

	if (Query.exec("SELECT ID FROM targets"))
		while (Query.next()) AddDevice(Query.value(0).toUInt());

	UpdateDevices();

	connect(&Serial,
		   SIGNAL(readyRead()),
		   SLOT(UpdatehData()));

	connect(DialogAddSensor,
		   SIGNAL(onWidgetAdd(unsigned char)),
		   SLOT(AddSensor(unsigned char)));

	connect(DialogAddEvent,
		   SIGNAL(onWidgetAdd(unsigned char)),
		   SLOT(AddEvent(unsigned char)));

	connect(DialogAddDevice,
		   SIGNAL(onWidgetAdd(unsigned char)),
		   SLOT(AddDevice(unsigned char)));
}

MainWindow::~MainWindow()
{
	Disconnect();

	foreach (SensorWidget* widget, Sensors) delete widget;
	foreach (EventWidget* widget, Events) delete widget;
	foreach (DeviceWidget* widget, Devices) delete widget;

	QSettings INI("Sensors.ini", QSettings::IniFormat);

	INI.setIniCodec("UTF-8");

	INI.beginGroup("Connection");

	INI.setValue("Active", Interface->Active->isChecked());
	INI.setValue("Time", Interface->Time->value());

	delete DialogAddSensor;
	delete DialogAddEvent;
	delete DialogAddDevice;

	delete Interface;
}

void MainWindow::Connect(void)
{
	if (!Serial.open(QIODevice::ReadWrite))
		QMessageBox::warning(
					this,
					"Błąd",
					"Nie udało się nawiązać połączenia z wybranym urządzeniem");

	Interface->Alive->setChecked(Serial.isOpen());
	Interface->groupOptions->setEnabled(Serial.isOpen());
}

void MainWindow::Disconnect(void)
{
	if (Serial.isOpen())
	{
		Serial.write("\0\0\0", 3);
		Serial.flush();

		Serial.close();
	}

	Interface->Alive->setChecked(Serial.isOpen());
	Interface->groupOptions->setEnabled(Serial.isOpen());
}

void MainWindow::AddWidget(void)
{
	if (sender() == Interface->AddSensor)
	{
		DialogAddSensor->open();
	}
	else if (sender() == Interface->AddEvent)
	{
		DialogAddEvent->open();
	}
	else if (sender() == Interface->AddDevice)
	{
		DialogAddDevice->open();
	}
}

void MainWindow::DeleteWidget(unsigned char ID, unsigned char uType)
{
	switch (uType)
	{
		case TYPE_SENSOR:

			delete Sensors.value(ID);
			Sensors.remove(ID);

		break;
		case TYPE_EVENT:

			delete Sensors.value(ID);
			Sensors.remove(ID);

		break;
		case TYPE_DEVICE:

			delete Sensors.value(ID);
			Sensors.remove(ID);

		break;
		default: qDebug() << "Próba usunięcia nieznanego widgetu";
	}
}

void MainWindow::AddSensor(unsigned char ID)
{
	SensorWidget* widget = new SensorWidget(this, ID);

	Sensors.insert(ID, widget);

	Interface->layoutSensors->addWidget(widget);

	connect(widget,
		   SIGNAL(onWidgetDelete(unsigned char, unsigned char)),
		   SLOT(DeleteWidget(unsigned char,unsigned char)));
}

void MainWindow::AddEvent(unsigned char ID)
{
	EventWidget* widget = new EventWidget(this, ID);

	Events.insert(ID, widget);

	Interface->layoutEvents->addWidget(widget);

	connect(widget,
		   SIGNAL(onWidgetDelete(unsigned char, unsigned char)),
		   SLOT(DeleteWidget(unsigned char,unsigned char)));
}

void MainWindow::AddDevice(unsigned char ID)
{
	DeviceWidget* widget = new DeviceWidget(this, ID);

	Devices.insert(ID, widget);

	Interface->layoutDevices->addWidget(widget);

	connect(widget,
		   SIGNAL(onWidgetDelete(unsigned char, unsigned char)),
		   SLOT(DeleteWidget(unsigned char,unsigned char)));
}

void MainWindow::UpdateDevices(void)
{
	Interface->Device->clear();

	foreach (const QSerialPortInfo& info, QSerialPortInfo::availablePorts())
	{
		Interface->Device->addItem(info.portName());
	}
}

void MainWindow::UpdateLink(void)
{
	if (sender() == Interface->Connect)
	{
		Disconnect();

		Serial.setPortName(Interface->Device->currentText());

		Connect();
	}
	else	if (sender() == Interface->Disconnect)
	{
		Disconnect();
	}
	else if (sender() == Interface->Save)
	{
		if (Serial.isOpen())
		{
			unsigned Time = (Interface->Time->value() * 65535) / 4.19424;

			unsigned char Frame[] = {
				Interface->Active->isChecked(),
				(unsigned char) Time,
				(unsigned char) (Time >> 8)
			};

			Serial.write((char*) Frame, 3);
		}
	}
}

void MainWindow::UpdatehData(void)
{
	static unsigned char aData[FRAME_SIZE];

	if (Serial.bytesAvailable() < FRAME_SIZE) return;

	Serial.read((char*) aData, FRAME_SIZE);

	for (unsigned i = 0; i < (FRAME_SIZE / 2); i++)
	{
		Engine.globalObject().setProperty(
					QString("x%1").arg(i + 1),
					((aData[2*i + 1] << 8) + aData[2*i]) * (5 / 1024.0));
	}

	emit onRefresh(Engine);
}

QSqlDatabase& MainWindow::getDatabase(void)
{
	return Database;
}

QScriptEngine& MainWindow::getEngine(void)
{
	return Engine;
}

MainWindow* MainWindow::getInstance(void)
{
	return Instance;
}
