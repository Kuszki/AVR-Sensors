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

	public:

		struct EventBinary
		{
			unsigned char Pin;
			unsigned char When;

			unsigned short Value;
		};

	private:

		const unsigned char ID;

		Ui::EventWidget* Interface;

		EventDialog* Dialog;

	public:

		explicit EventWidget(QWidget* parent, unsigned char uID);
		~EventWidget();

	private slots:

		void onOptionsClick(void);
		void onDeleteClick(void);

		void onDialogSave(const EventDialog::EventData& tData);

	public slots:

		void onUpdateData(void);

	signals:

		void onDataChange(void);

		void onWidgetDelete(unsigned char uID,
						unsigned char uWT);

};

#endif // EVENTWIDGET_HPP
