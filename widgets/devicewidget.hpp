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

	public:

		DeviceWidget(QWidget* parent, unsigned char uID);
		~DeviceWidget();

	public slots:

		void onOptionsClick(void);
		void onDeleteClick(void);
		void onDialogSave(const DeviceDialog::DeviceData& tData);

	signals:

		void onDataChange(void);
		void onWidgetDelete(unsigned char uID,
						unsigned char uWT);

};

#endif // DEVICEWIDGET_HPP
