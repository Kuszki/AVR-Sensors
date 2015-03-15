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

	INI.beginGroup("Measurements");

	Interface->Average->setChecked(INI.value("Average", false).toBool());
	Interface->Samples->setValue(INI.value("Samples", 10).toUInt());

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

	if (Database.isOpenError()) qFatal("Nie można połączyć się z bazą danych");

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

	INI.beginGroup("Measurements");

	INI.setValue("Average", Interface->Average->isChecked());
	INI.setValue("Samples", Interface->Samples->value());

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
	Interface->Save->setEnabled(Serial.isOpen());
}

void MainWindow::Disconnect(void)
{
	if (Serial.isOpen())
	{
		Serial.write("\0\0\0\0\0", SIGNAL_SIZE);
		Serial.flush();

		Serial.close();
	}

	Interface->Alive->setChecked(Serial.isOpen());
	Interface->Save->setEnabled(Serial.isOpen());

	emit onControlChange(false);
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

	widget->onUpdateSample(Interface->Average->isChecked(),
					   Interface->Samples->value());

	connect(widget,
		   SIGNAL(onWidgetDelete(unsigned char, unsigned char)),
		   SLOT(DeleteWidget(unsigned char,unsigned char)));

	connect(widget,
		   SIGNAL(onDataChange()),
		   SLOT(UpdateEvents()));
}

void MainWindow::AddEvent(unsigned char ID)
{
	EventWidget* widget = new EventWidget(this, ID);

	Events.insert(ID, widget);

	Interface->layoutEvents->addWidget(widget);

	connect(widget,
		   SIGNAL(onWidgetDelete(unsigned char, unsigned char)),
		   SLOT(DeleteWidget(unsigned char,unsigned char)));

	/*connect(widget,
		   SIGNAL(onSwitchEvent(unsigned char,bool)),
		   SLOT(SwitchDevice(unsigned char,bool)));*/
}

void MainWindow::AddDevice(unsigned char ID)
{
	DeviceWidget* widget = new DeviceWidget(this, ID);

	Devices.insert(ID, widget);

	Interface->layoutDevices->addWidget(widget);

	connect(widget,
		   SIGNAL(onWidgetDelete(unsigned char, unsigned char)),
		   SLOT(DeleteWidget(unsigned char,unsigned char)));

	connect(widget,
		   SIGNAL(onDataChange()),
		   SLOT(UpdateEvents()));

	connect(widget,
		   SIGNAL(onManualSwitch(unsigned char,bool)),
		   SLOT(SwitchDevice(unsigned char,bool)));
}

void MainWindow::UpdateEvents(void)
{
	foreach (EventWidget* widget, Events) widget->onUpdateData();
}

void MainWindow::UpdateControl(void)
{
	if (Serial.isOpen())
	{
		unsigned char Frame[SIGNAL_SIZE];

		Frame[0] = SIGNAL_CONTROL;
		Frame[1] = (Interface->controlAuto->isChecked() << 2) |
				 (Interface->controlRemote->isChecked() << 1);

		Serial.write((char*) Frame, SIGNAL_SIZE);

		emit onControlChange(Interface->controlManual->isChecked());
	}
}

void MainWindow::UpdateMeasurements(void)
{
	emit onSampleUpdate(Interface->Average->isChecked(),
					Interface->Samples->value());
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
			unsigned char Frame[SIGNAL_SIZE];

			unsigned Time = (Interface->Time->value() * 65535) / 4.19424;

			Frame[0] = Interface->Active->isChecked() ?
						 SIGNAL_START : SIGNAL_STOP;
			Frame[1] = (unsigned char) Time;
			Frame[2] = (unsigned char) (Time >> 8);

			Serial.write((char*) Frame, SIGNAL_SIZE);

			UpdateControl();
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

	emit onRefreshValues(Engine);
}

void MainWindow::SwitchDevice(unsigned char uPin, bool bState)
{
	if (Serial.isOpen())
	{
		unsigned char Frame[SIGNAL_SIZE];

		Frame[0] = SIGNAL_CONTROL;
		Frame[1] = uPin;
		Frame[2] = bState;

		Serial.write((char*) Frame, SIGNAL_SIZE);
	}
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
