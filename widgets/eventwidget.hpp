#ifndef EVENTWIDGET_HPP
#define EVENTWIDGET_HPP

#include <QWidget>

namespace Ui
{
	class EventWidget;
}

class EventWidget : public QWidget
{

		Q_OBJECT

	public:
		explicit EventWidget(QWidget *parent = 0);
		~EventWidget();

	private:
		Ui::EventWidget* Interface;

};

#endif // EVENTWIDGET_HPP
