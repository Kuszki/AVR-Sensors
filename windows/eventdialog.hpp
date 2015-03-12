#ifndef EVENTDIALOG_HPP
#define EVENTDIALOG_HPP

#include <QDialog>

namespace Ui {
	class EventDialog;
}

class EventDialog : public QDialog
{
		Q_OBJECT

	public:
		explicit EventDialog(QWidget *parent = 0);
		~EventDialog();

	private:
		Ui::EventDialog *ui;
};

#endif // EVENTDIALOG_HPP
