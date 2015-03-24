#ifndef EVENTDIALOG_HPP
#define EVENTDIALOG_HPP

#include <QDialog>
#include <QSettings>
#include <QDebug>
#include <QPushButton>
#include <QScriptEngine>
#include <QMessageBox>
#include <QRegularExpression>

#include <QtSql>

namespace Ui
{
	class EventDialog;
}

class EventDialog : public QDialog
{

		Q_OBJECT

	public:

		struct EventData
		{
			QString Name;

			float Value;
			float Voltage;

			bool Active;
			bool Action;
			bool Where;
			bool Simple;

			QString Sensor;
			QString Device;
			QString Variable;

			unsigned char SensorID;
			unsigned char DeviceID;
			unsigned char PinID;
			unsigned char SeID;
		};

	private:

		const unsigned char ID;

		Ui::EventDialog* Interface;

		EventData DefaultData;
		EventData LastData;

		void GetData(EventData& tData);
		void SetData(EventData& tData,
				   bool bRefresh);

		void CompleteData(EventData& tData);

	public:

		EventDialog(QWidget* parent,
				  unsigned char uID);
		~EventDialog();

		bool LoadSettings(void);
		bool SaveSettings(void);
		bool DeleteSettings(void);

		void open(void);

	private slots:

		void accept(void);
		void reject(void);

		void onParamsChange(void);

	signals:

		void onSettingsAccept(const EventDialog::EventData& tData);

		void onWidgetAdd(unsigned char uID);
};

#endif // EVENTDIALOG_HPP
