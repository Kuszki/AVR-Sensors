#ifndef SETTINGSDIALOG_HPP
#define SETTINGSDIALOG_HPP

#include <QDialog>
#include <QSettings>
#include <QDebug>
#include <QPushButton>

namespace Ui
{
	class SettingsDialog;
}

class SettingsDialog : public QDialog
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

		Ui::SettingsDialog* Interface;

		SensorData tLastData;

		QString sSensor;

	public:

		explicit SettingsDialog(QWidget *parent = 0);
		~SettingsDialog();

		void LoadSettings(const QString& sSection = QString());

		void SaveSettings(const QString& sSection = QString());

		void GetData(SettingsDialog::SensorData& tData);

		void SetData(SettingsDialog::SensorData& tData,
				   bool bRefresh = true);

		void accept(void);

		void reject(void);

	private slots:

		void onParamsChange(void);

	signals:

		void onSettingsAccept(const SettingsDialog::SensorData& tData);

};

#endif // SETTINGSDIALOG_HPP
