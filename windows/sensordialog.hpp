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
			QString Desc;

			float Minimum;
			float Maximum;

			bool Active;
			bool Style;
			bool Virtual;
			bool Multiple;
		};

	private:

		const unsigned char ID;

		Ui::SensorDialog* Interface;

		SensorData DefaultData;
		SensorData LastData;

		void GetData(SensorData& tData);
		void SetData(SensorData& tData,
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
		void onWidgetAdd(unsigned char uID);

};

#endif // SETTINGSDIALOG_HPP
