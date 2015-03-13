#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#define FRAME_SIZE 12

#define TYPE_SENSOR 1
#define TYPE_EVENT 2
#define TYPE_DEVICE 3

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QString>
#include <QMessageBox>
#include <QDebug>
#include <QByteArray>
#include <QMap>

#include <QtSql>

#include "widgets/sensorwidget.hpp"
#include "widgets/eventwidget.hpp"
#include "widgets/devicewidget.hpp"

namespace Ui
{
	class MainWindow;
}

class MainWindow : public QMainWindow
{

		Q_OBJECT

	public:

		explicit MainWindow(QWidget* parent = nullptr);
		~MainWindow();

	private:

		static MainWindow* Instance;

		Ui::MainWindow* Interface;

		QSerialPort Serial;
		QScriptEngine Engine;
		QSqlDatabase Database;

		SensorDialog* DialogAddSensor;
		EventDialog* DialogAddEvent;
		DeviceDialog* DialogAddDevice;

		QMap<unsigned char, SensorWidget*> Sensors;
		QMap<unsigned char, EventWidget*> Events;
		QMap<unsigned char, DeviceWidget*> Devices;

		void Connect(void);
		void Disconnect(void);

	public:

		static MainWindow* getInstance(void);

		QSqlDatabase& getDatabase(void);
		QScriptEngine& getEngine(void);

	public slots:

		void AddWidget(void);
		void DeleteWidget(unsigned char ID,
					   unsigned char uType);

		void AddSensor(unsigned char ID);
		void AddEvent(unsigned char ID);
		void AddDevice(unsigned char ID);

		void UpdateDevices(void);
		void UpdateLink(void);
		void UpdatehData(void);

	signals:

		void onRefresh(QScriptEngine& Engine);

};

#endif // MAINWINDOW_HPP
