#include "mainwindow.hpp"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
: QMainWindow(parent), Interface(new Ui::MainWindow)
{
	Interface->setupUi(this);

	sSerial = new QSerialPort();

	Sensors = new SensorWidget*[5];

	for (int i = 0; i < 1; i++)
	{
		Sensors[i] = new SensorWidget(this);

		Interface->layoutSensors->addWidget(Sensors[i]);
	}

	sSerial->setBaudRate(QSerialPort::Baud9600);
	sSerial->setParity(QSerialPort::NoParity);
	sSerial->setStopBits(QSerialPort::OneStop);

	UpdateDevices();

	connect(sSerial, SIGNAL(readyRead()), SLOT(ReadData()));
}

MainWindow::~MainWindow()
{
	if (sSerial->isOpen())
	{
		sSerial->write("\0\0\0", 3);
		sSerial->flush();

		sSerial->close();
	}

	delete Interface;

	delete [] Sensors;
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
	if (sSerial->isOpen())
	{
		sSerial->write("\0\0\0", 3);
		sSerial->flush();

		sSerial->close();
	}

	if (sender() == Interface->Connect)
	{

		sSerial->setPortName(Interface->Device->currentText());

		if (!sSerial->open(QIODevice::ReadWrite))
		{
			QMessageBox::warning(this, "Błąd", "Nie udało się nawiązać połączenia z wybranym urządzeniem");
		}

	}

	Interface->Alive->setChecked(sSerial->isOpen());
	Interface->groupOptions->setEnabled(sSerial->isOpen());
}

void MainWindow::UpdateSettings(void)
{
	if (!sSerial->isOpen()) return;

	unsigned Time = (65535 * Interface->Time->value()) / 4.19424;

	unsigned char Frame[] = {
		Interface->Active->isChecked(),
		(unsigned char) Time & 0xFF,
		(unsigned char) (Time >> 8) & 0xFF
	};

	sSerial->write((char*) Frame, 3);
}

void MainWindow::ReadData(void)
{
	if (sSerial->bytesAvailable() < 3) return;

	unsigned char aData[3];

	sSerial->read((char*) aData, 3);

	unsigned ID = aData[0];
	unsigned Value = (aData[2] << 8) + aData[1];

	if (ID < 5)
	{
		Sensors[ID]->UpdateValue(Value);
	}

	qDebug() << "Update data, sensor = " << ID << " data = " << Value;
}
