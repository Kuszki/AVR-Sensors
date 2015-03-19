#ifndef DEVICEWIDGET_HPP
#define DEVICEWIDGET_HPP

#include <QWidget>
#include <QScriptEngine>
#include <QRegularExpression>
#include <QDebug>

#include "windows/devicedialog.hpp"

namespace Ui
{
	class DeviceWidget;
}

class DeviceWidget : public QWidget
{

		Q_OBJECT

	private:

		const unsigned char ID;

		Ui::DeviceWidget* Interface;

		DeviceDialog* Dialog;

		unsigned char uPin = 0;
		bool bInitialSwitch = false;

	public:

		DeviceWidget(QWidget* parent, unsigned char uID);
		~DeviceWidget();

	private slots:

		void onActiveSwitch(bool bMode);

		void onOptionsClick(void);
		void onDeleteClick(void);

		void onDialogSave(const DeviceDialog::DeviceData& tData);

	public slots:

		void onChangeControl(bool bLock);

		void onChangeState(unsigned char uPinID,
					    bool bState);

	signals:

		void onDataChange(void);

		void onManualSwitch(unsigned char uDev,
						bool bState);

		void onWidgetDelete(unsigned char uID,
						unsigned char uWT);

};

#endif // DEVICEWIDGET_HPP
