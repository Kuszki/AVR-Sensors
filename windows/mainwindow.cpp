#include "mainwindow.hpp"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
: QMainWindow(parent), Interface(new Ui::MainWindow), uSensors(10)
{
	Interface->setupUi(this);

	Engine = new QScriptEngine();

	sSerial = new QSerialPort();

	sSerial->setBaudRate(QSerialPort::Baud9600);
	sSerial->setParity(QSerialPort::NoParity);
	sSerial->setStopBits(QSerialPort::OneStop);

	Sensors = new SensorWidget*[uSensors];

	for (int i = 0; i < uSensors; i++) Sensors[i] = nullptr;

	QSettings INI("Sensors.ini", QSettings::IniFormat);

	INI.setIniCodec("UTF-8");
	INI.beginGroup("App");

	UpdateDevices();

	Interface->Active->setChecked(INI.value("Active", false).toBool());
	Interface->Time->setValue(INI.value("Time", 3.0).toDouble());

	connect(sSerial, SIGNAL(readyRead()), SLOT(UpdatehData()));
}

MainWindow::~MainWindow()
{
	Disconnect();

	QSettings INI("Sensors.ini", QSettings::IniFormat);

	INI.setIniCodec("UTF-8");
	INI.beginGroup("App");

	INI.setValue("Active", Interface->Active->isChecked());
	INI.setValue("Time", Interface->Time->value());

	QThread::currentThread()->sleep(1);

	delete Interface;
	delete Engine;
	delete sSerial;

	delete [] Sensors;
}

void MainWindow::Connect(void)
{
	if (!sSerial->open(QIODevice::ReadWrite))
		QMessageBox::warning(this,
						 "Błąd",
						 "Nie udało się nawiązać połączenia z wybranym urządzeniem");

	Interface->Alive->setChecked(sSerial->isOpen());
	Interface->groupOptions->setEnabled(sSerial->isOpen());
}

void MainWindow::Disconnect(void)
{
	if (sSerial->isOpen())
	{
		sSerial->write("\0\0\0", 3);
		sSerial->flush();

		sSerial->close();
	}

	Interface->Alive->setChecked(sSerial->isOpen());
	Interface->groupOptions->setEnabled(sSerial->isOpen());
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

		sSerial->setPortName(Interface->Device->currentText());

		Connect();
	}
	else	if (sender() == Interface->Disconnect)
	{
		Disconnect();
	}
	else if (sender() == Interface->Save)
	{
		if (sSerial->isOpen())
		{
			unsigned Time = (Interface->Time->value() * 65535) / 4.19424;

			unsigned char Frame[] = {
				Interface->Active->isChecked(),
				(unsigned char) Time & 0xFF,
				(unsigned char) (Time >> 8) & 0xFF
			};

			sSerial->write((char*) Frame, 3);
		}
	}
}
/*
void MainWindow::UpdateCount(int iCount)
{
	for (unsigned char i = 0; i < uSensors; i++)
		if (Sensors[i])
		{
			delete Sensors[i];

			Sensors[i] = nullptr;
		}

	uSensors = iCount;

	for (unsigned char i = 0; i < uSensors; i++)
	{
		Sensors[i] = new SensorWidget(this, i);

		Interface->layoutSensors->addWidget(Sensors[i]);
	}
}
*/
void MainWindow::UpdatehData(void)
{
	static unsigned char aData[FRAME_SIZE];

	if (sSerial->bytesAvailable() < FRAME_SIZE) return;

	sSerial->read((char*) aData, FRAME_SIZE);

	for (unsigned i = 0; i < (FRAME_SIZE / 2); i++)
	{
		const QString sLabel = "x" + QString::number(i + 1);
		const float fValue = ((aData[2*i + 1] << 8) + aData[2*i]) * (5 / 1024.0);

		Engine->globalObject().setProperty(sLabel, fValue, QScriptValue::ReadOnly);
	}

	emit onRefresh(Engine);
}
