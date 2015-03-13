#ifndef EVENTDIALOG_HPP
#define EVENTDIALOG_HPP

#include <QDialog>

namespace Ui
{
	class EventDialog;
}

class EventDialog : public QDialog
{

		Q_OBJECT

	public:
		EventDialog(QWidget* parent,
				  unsigned char uID);
		~EventDialog();

	private:

		const unsigned char ID;

		Ui::EventDialog* Interface;
};

#endif // EVENTDIALOG_HPP
