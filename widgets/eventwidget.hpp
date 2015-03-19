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
			QString Variable;

			unsigned char Pin;
			unsigned char Value;

			float SwitchValue;
			float LastValue;

			bool Active;
			bool Action;
			bool When;
			bool Simple;
			bool Switch;
		};

	private:

		const unsigned char ID;

		Ui::EventWidget* Interface;

		EventDialog* Dialog;

		EventBinary Data;

	public:

		explicit EventWidget(QWidget* parent, unsigned char uID);
		~EventWidget();

	private slots:

		void onOptionsClick(void);
		void onDeleteClick(void);

		void onDialogSave(const EventDialog::EventData& tData);

	public slots:

		void onUpdateValue(QScriptEngine& Engine);

		void onUpdateControl(bool bControl);

		void onUpdateData(void);

	signals:

		void onSwitchEvent(unsigned char uID,
					    bool bState);

		void onWidgetDelete(unsigned char uID,
						unsigned char uWT);

};

#endif // EVENTWIDGET_HPP
