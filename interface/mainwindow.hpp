#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#define FRAME_SIZE 12

#include <QMainWindow>
#include <QSerialPort>
#include <QString>
#include <QMessageBox>
#include <QSerialPortInfo>
#include <QDebug>
#include <QByteArray>
#include <QThread>

#include "interface/sensorwidget.hpp"

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

		SensorWidget** Sensors;

		QScriptEngine* Engine;

		QSerialPort* sSerial;

		unsigned char uSensors;

		void Connect(void);

		void Disconnect(void);

	public slots:

		void UpdateDevices(void);

		void UpdateLink(void);

		void UpdateCount(int iCount);

		void UpdatehData(void);

	signals:

		void onRefresh(QScriptEngine* Eng);

};

#endif // MAINWINDOW_HPP
