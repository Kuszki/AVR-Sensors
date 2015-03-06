#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QSerialPort>
#include <QString>
#include <QMessageBox>
#include <QSerialPortInfo>
#include <QDebug>
#include <QByteArray>

#include "sensorwidget.hpp"

namespace Ui
{
	class MainWindow;
}

class MainWindow : public QMainWindow
{

		Q_OBJECT

	public:

		explicit MainWindow(QWidget *parent = 0);
		~MainWindow();

	private:

		Ui::MainWindow* Interface;

		QSerialPort* sSerial;

		SensorWidget** Sensors;

		unsigned char uSensors;

		void Connect(void);

		void Disconnect(void);

	public slots:

		void UpdateDevices(void);

		void UpdateLink(void);

		void UpdateCount(int iCount);

		void ReadData(void);

};

#endif // MAINWINDOW_HPP
