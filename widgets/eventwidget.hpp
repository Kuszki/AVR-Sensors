#ifndef EVENTWIDGET_HPP
#define EVENTWIDGET_HPP

#include <QWidget>
#include <QScriptEngine>
#include <QRegularExpression>
#include <QDebug>

#include "windows/eventdialog.hpp"

namespace Ui
{
	class EventWidget;
}

class EventWidget : public QWidget
{

		Q_OBJECT

	private:

		const unsigned char ID;

		Ui::EventWidget* Interface;

		EventDialog* Dialog;

	public:

		explicit EventWidget(QWidget* parent, unsigned char uID);
		~EventWidget();

	public slots:

		void onOptionsClick(void);
		void onDeleteClick(void);
		void onDialogSave(const EventDialog::EventData& tData);

	signals:

		void onWidgetDelete(unsigned char uID,
						unsigned char uWT);

};

#endif // EVENTWIDGET_HPP
