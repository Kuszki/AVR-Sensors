#ifndef DEVICEDIALOG_HPP
#define DEVICEDIALOG_HPP

#include <QDialog>

namespace Ui {
	class DeviceDialog;
}

class DeviceDialog : public QDialog
{
		Q_OBJECT

	public:
		explicit DeviceDialog(QWidget *parent = 0);
		~DeviceDialog();

	private:
		Ui::DeviceDialog *ui;
};

#endif // DEVICEDIALOG_HPP
