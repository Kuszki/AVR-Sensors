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

#include "sensor/sensor.hpp"

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

	public slots:

		void UpdateDevices(void);

		void UpdateLink(void);

		void UpdateSettings(void);

		void ReadData(void);

};

#endif // MAINWINDOW_HPP
