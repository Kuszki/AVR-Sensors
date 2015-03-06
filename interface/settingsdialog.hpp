#ifndef SETTINGSDIALOG_HPP
#define SETTINGSDIALOG_HPP

#include <QDialog>
#include <QSettings>
#include <QDebug>

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

	public:

		explicit SettingsDialog(QWidget *parent = 0);
		~SettingsDialog();

		void LoadSettings(const QString& sFile,
					   const QString& sSection);

	private slots:

		void onDialogSave(void);

	signals:

		void onSettingsAccept(const SettingsDialog::SensorData& tData);

};

#endif // SETTINGSDIALOG_HPP
