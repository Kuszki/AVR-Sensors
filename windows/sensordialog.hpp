#ifndef SETTINGSDIALOG_HPP
#define SETTINGSDIALOG_HPP

#include <QDialog>
#include <QSettings>
#include <QDebug>
#include <QPushButton>
#include <QScriptEngine>
#include <QMessageBox>

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

		Ui::SensorDialog* Interface;

		SensorData tLastData;

		QString sSensor;

	public:

		explicit SensorDialog(QWidget* parent = nullptr);
		~SensorDialog();

		void LoadSettings(const QString& sSection = QString());

		void SaveSettings(const QString& sSection = QString());

		void GetData(SensorDialog::SensorData& tData);

		void SetData(SensorDialog::SensorData& tData,
				   bool bRefresh = true);

		void accept(void);

		void reject(void);

	private slots:

		void onParamsChange(void);

	signals:

		void onSettingsAccept(const SensorDialog::SensorData& tData);

};

#endif // SETTINGSDIALOG_HPP
