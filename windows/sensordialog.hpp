#ifndef SETTINGSDIALOG_HPP
#define SETTINGSDIALOG_HPP

#include <QDialog>
#include <QSettings>
#include <QDebug>
#include <QPushButton>
#include <QScriptEngine>
#include <QMessageBox>
#include <QtSql>

namespace Ui
{
	class SensorDialog;
}

class SensorDialog : public QDialog
{

		Q_OBJECT

	public:

		struct SensorData
		{
			QString Name;
			QString Equation;
			QString Label;

			float Minimum;
			float Maximum;

			bool Active;
			bool Style;
		};

	private:

		const unsigned char ID;

		Ui::SensorDialog* Interface;

		SensorDialog::SensorData DefaultData;
		SensorDialog::SensorData LastData;

		void GetData(SensorDialog::SensorData& tData);
		void SetData(SensorDialog::SensorData& tData,
				   bool bRefresh);

	public:

		SensorDialog(QWidget* parent,
				   unsigned char uID);
		~SensorDialog();

		bool LoadSettings(void);
		bool SaveSettings(void);
		bool DeleteSettings(void);

		void open(void);

	private slots:

		void accept(void);
		void reject(void);

		void onParamsChange(void);

	signals:

		void onSettingsAccept(const SensorDialog::SensorData& tData);
		void onSensorAdd(unsigned char uID);

};

#endif // SETTINGSDIALOG_HPP
