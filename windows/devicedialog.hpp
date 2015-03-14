#ifndef DEVICEDIALOG_HPP
#define DEVICEDIALOG_HPP

#include <QDialog>
#include <QSettings>
#include <QDebug>
#include <QPushButton>
#include <QScriptEngine>
#include <QMessageBox>
#include <QtSql>

namespace Ui {
	class DeviceDialog;
}

class DeviceDialog : public QDialog
{

		Q_OBJECT

	public:

		struct DeviceData
		{
			QString Name;

			unsigned Pin;

			bool Active;
		};

	private:

		const unsigned char ID;

		Ui::DeviceDialog* Interface;

		DeviceData DefaultData;
		DeviceData LastData;

		void GetData(DeviceData& tData);
		void SetData(DeviceData& tData,
				   bool bRefresh);

	public:
		DeviceDialog(QWidget* parent, unsigned char uID);
		~DeviceDialog();

		bool LoadSettings(void);
		bool SaveSettings(void);
		bool DeleteSettings(void);

		void open(void);

	private slots:

		void accept(void);
		void reject(void);

		void onParamsChange(void);

	signals:

		void onSettingsAccept(const DeviceDialog::DeviceData& tData);
		void onWidgetAdd(unsigned char uID);
};

#endif // DEVICEDIALOG_HPP
