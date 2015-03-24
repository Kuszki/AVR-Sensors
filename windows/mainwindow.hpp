#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#define V_MIN 0.0
#define V_MAX 5.0
#define V_RES 1024
#define V_QUA (V_MAX / V_RES)

#define SENSORS_COUNT 6

#define FRAME_SIZE ((SENSORS_COUNT * 2) + 1)
#define SIGNAL_SIZE 5

#define TYPE_SENSOR 1
#define TYPE_EVENT 2
#define TYPE_DEVICE 3

#define SIGNAL_STOP			0
#define SIGNAL_START		1
#define SIGNAL_CONTROL		2
#define SIGNAL_SWITCH		3
#define SIGNAL_EVENTS		4
#define SIGNAL_DEVICES		5

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

		int uSample = 0;

		void Connect(void);
		void Disconnect(void);

	public:

		explicit MainWindow(QWidget* parent = nullptr);
		~MainWindow();

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

		void UpdateEvents(void);

		void UpdateControl(void);
		void UpdateMeasurements(void);
		void UpdateDevices(void);
		void UpdateLink(void);
		void UpdatehData(void);

		void SaveEvents(void);
		void SaveDevices(void);

		void SwitchDevice(unsigned char uPin,
					   bool bState);

	signals:

		void onControlChange(unsigned uControl);

		void onRefreshValues(QScriptEngine& Engine);
		void onRefreshEvents(QScriptEngine& Engine);
		void onRefreshDevices(unsigned char Devices);

		void onSampleUpdate(bool bActive, unsigned uSamples);

		void onDeviceUpdate(unsigned char uPin,
						bool bState);

};

#endif // MAINWINDOW_HPP
